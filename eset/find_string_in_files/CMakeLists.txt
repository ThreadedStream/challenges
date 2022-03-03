cmake_minimum_required(VERSION 3.19)

project(esetassignment CXX)

set(SOURCES src/main.cpp)
set(THREADS_PREFER_PTHREAD FLAG ON)
find_package(Threads REQUIRED)

add_executable(esetass ${SOURCES})
target_link_libraries(esetass PRIVATE Threads::Threads)
set_property(TARGET esetass PROPERTY CXX_STANDARD 17)