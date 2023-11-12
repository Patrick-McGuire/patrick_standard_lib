cmake_minimum_required(VERSION 3.22)
project(patrick_standard_lib)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(patrick_standard_lib main.cpp include/psl.h include/Array.h include/String.h include/Converter.h)

