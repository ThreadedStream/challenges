cmake_minimum_required(VERSION 3.19)

project(esetassignment CXX)

set(SOURCES src/main.cpp)

add_executable(esetass ${SOURCES})
set_property(TARGET esetass PROPERTY CXX_STANDARD 17)