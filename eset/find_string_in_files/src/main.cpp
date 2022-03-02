#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <utility>

namespace fs = std::filesystem;

struct MatchInfo {
    std::string filename;
    size_t offset;
    std::string prefix;
    std::string suffix;
};

// set 1 GB file size limit.
// files below this limit may be loaded into memory
static const size_t FILE_SIZE_LIMIT = 1024 * 1024 * 1024;

std::vector<MatchInfo> process_small_file(const fs::directory_entry& entry, const std::string& target) {
    const auto& path_str = entry.path().string();
    const auto file_size = entry.file_size();
    std::ifstream stream(path_str, std::ios_base::binary);
    if (!stream) {
        std::cerr << "could not open a file " << path_str << '\n';
        return std::vector<MatchInfo>();
    }
    std::string buffer;
    buffer.resize(file_size);
    stream.read(buffer.data(), file_size);
    buffer.data()[file_size] = '\0';
    std::string prefix;
    std::string suffix;

    std::vector<MatchInfo> positions;

    auto pos = buffer.find(target, 0);
    while (pos != std::string::npos) {
        if (pos - 3 > 0) {
            prefix = buffer.substr(pos-3, 3);
        }
        if (pos + target.size() + 3 < file_size) {
            suffix = buffer.substr(pos+target.size(),3);
        }
        positions.push_back(MatchInfo{path_str, pos, prefix, suffix});
        pos = buffer.find(target, pos+1);
    }
    return positions;
}

std::vector<MatchInfo> process(const fs::directory_entry& entry, const std::string& target) {
    if (entry.file_size() < FILE_SIZE_LIMIT) {
        return process_small_file(entry, target);
    }
}

void format_output(const std::vector<MatchInfo>& v, const std::string &target) {
    for (const auto& c : v) {
        std::cout << c.filename << "(" << c.offset << "): " << c.prefix << " " << target << " " << c.suffix << '\n';
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
            format_output(vec, target);
        }
    }
}

