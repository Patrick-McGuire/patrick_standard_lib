cmake_minimum_required(VERSION 3.24)
project(PMcGuireStandard)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(PMcGuireStandard main.cpp include/pmcguire.h)
