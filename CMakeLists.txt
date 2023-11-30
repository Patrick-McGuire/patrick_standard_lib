cmake_minimum_required(VERSION 3.22)
project(patrick_standard_lib)

set(CMAKE_CXX_STANDARD 11)


add_executable(patrick_standard_lib main.cpp src/psl.h src/Array.h src/Converter.h src/Messaging.h src/pub_sub/Manager.h src/pub_sub/Subscriber.h src/pub_sub/Publisher.h src/RingBuffer.h src/Function.h src/String.h src/StringDef.h)
#add_executable(patrick_standard_lib main.cpp)
