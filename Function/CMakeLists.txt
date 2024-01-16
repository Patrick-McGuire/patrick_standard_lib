cmake_minimum_required(VERSION 3.24)
project(Function)

set(CMAKE_CXX_STANDARD 11)

add_executable(Function main.cpp pfunction.h MethodBinder.h)
