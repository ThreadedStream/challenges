#include <filesystem>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <sstream>
#include <utility>
#include <chrono>
#include <thread>
#include <mutex>

namespace fs = std::filesystem;
using i32 = int32_t;
using i64 = int64_t;

// set 1 GB file size limit.
// files below this limit may be loaded into memory
static const size_t FILE_SIZE_LIMIT = 1024 * 1024 * 1024;
static const size_t CHUNK_SIZE = 16384;
static std::mutex fread_mutex;
static std::mutex format_mutex;

struct MatchInfo {
    std::string filename;
    i32 offset;
    std::string prefix;
    std::string suffix;
};


void format_string(const std::string& str); 
void format_output(const std::vector<MatchInfo>& v, const std::string &target);

std::vector<MatchInfo> process_chunk(std::string& chunk, const std::string& target, const std::string& path) {
    std::string prefix, suffix;
    std::vector<MatchInfo> positions;
    auto pos = static_cast<i32>(chunk.find(target, 0));
    while (pos != std::string::npos) {
        if (pos - 3 >= 0) {
            prefix = chunk.substr(pos-3, 3);
        } else {
            prefix = "(null)";
        }
        if (pos + target.size() + 3 < chunk.size()) {
            suffix = chunk.substr(pos+target.size(),3);
        } else {
            suffix = "(null)";
        }
        positions.push_back(MatchInfo{path, pos, prefix, suffix});
        pos = chunk.find(target, pos+1);
    }
    return positions;
}

std::vector<MatchInfo> process_small_file(const fs::directory_entry& entry, const std::string& target) {
    const auto& path_str = entry.path().string();
    const auto file_size = entry.file_size();
    std::ifstream stream(path_str);
    if (!stream) {
        std::cerr << "could not open a file " << path_str << '\n';
        return std::vector<MatchInfo>();
    }
    std::string buffer;
    buffer.resize(file_size);
    stream.read(buffer.data(), file_size);
    buffer.data()[file_size-1] = '\0';
    return process_chunk(buffer, target, path_str);
}

void read_chunk(std::ifstream& stream, const std::string& target, const std::string& path, const i64 curr_chunk_size) {
    std::string chunk;
    chunk.resize(curr_chunk_size);
    fread_mutex.lock();
    stream.readsome(chunk.data(), curr_chunk_size);
    fread_mutex.unlock();
    chunk.data()[curr_chunk_size-1] = '\0';
    const auto out = process_chunk(chunk, target, path);
    format_output(out, target);
}

std::vector<MatchInfo> process_large_file(const fs::directory_entry& entry, const std::string& target) {
    std::cout << "it's really a large one\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    const auto& path_str = entry.path().string();
    std::ifstream stream(path_str);
    if (!stream) {
        std::cerr << "could not open a file " << path_str << '\n';
        return std::vector<MatchInfo>();
    }
    std::vector<std::thread> thread_pool;
    auto left_so_far = entry.file_size();
    i64 curr_chunk_size = 0;
    while (left_so_far > 0) {
        if (left_so_far - CHUNK_SIZE < 0) {
            curr_chunk_size = left_so_far;
        } else {
            curr_chunk_size = CHUNK_SIZE;
        }
        left_so_far = left_so_far - CHUNK_SIZE;
        thread_pool.push_back(
            std::thread(read_chunk, std::ref(stream), target, path_str, curr_chunk_size)
        );
    }

    std::cout << "got here\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    for (auto &thread : thread_pool) {
        thread.join();
    }
    return std::vector<MatchInfo>();
}

std::vector<MatchInfo> process(const fs::directory_entry& entry, const std::string& target) {
    if (entry.file_size() < FILE_SIZE_LIMIT) {
        return process_small_file(entry, target);
    }
    return process_large_file(entry, target);
}

void format_output(const std::vector<MatchInfo>& v, const std::string &target) {
    format_mutex.lock();
    for (const auto& c : v) {
        std::cout << c.filename << "(" << c.offset << "): ";
        format_string(c.prefix);
        std::cout << " " << target << " ";
        format_string(c.suffix);
        std::putchar('\n');
    }
    format_mutex.unlock();
}

void format_string(const std::string& str) {
    for (const auto &c : str) {
        switch (c) {
            case '\t':
                std::printf("\\t");
                break;
            case '\n':
                std::printf("\\n");
                break;
            case '\r':
                std::printf("\\r");
                break;
            case '\b':
                std::printf("\\b");
            case ' ':
                std::printf("<space>");
                break;
            default:
                std::putchar(c);
                break;
        }
    }
}

int main(int argc, const char* argv[]) {
    if (argc < 3) {
        std::cerr << "usage: prog <path> <string_to_match>\n";
        return -1;
    }
    std::string path = argv[1];
    std::string target = argv[2];

    if (fs::is_directory(path)) {
        for (const auto &entry: fs::recursive_directory_iterator(path)) {
            if (fs::is_directory(entry)) {
                continue;
            }
            const auto vec = process(entry, target);
            if (vec.empty()) { continue; }
            format_output(vec, target);
        }
    } else {
        const auto vec = process(fs::directory_entry(path), target);
        format_output(vec, target);
    }
}

