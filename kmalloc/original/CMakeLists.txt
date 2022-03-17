cmake_minimum_required(VERSION 3.21)
project(kmalloc C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(tests)

add_library(kmalloc
        kmalloc.c)
#target_compile_options(kmalloc PUBLIC -m32)

add_library(kmalloc_shared SHARED kmalloc.c)
target_compile_options(kmalloc_shared PUBLIC -m32)
