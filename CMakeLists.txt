cmake_minimum_required(VERSION 3.22)
project(patrick_standard_lib)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-elide-constructors")

add_executable(patrick_standard_lib main.cpp src/psl.h src/Array2.h src/Converter.h src/Messaging.h src/pub_sub/Manager.h src/pub_sub/Subscriber.h src/pub_sub/Publisher.h src/RingBuffer.h src/Function.h src/String.h src/StringBaseDef.h src/SubStringDef.h src/Array.h src/ArrayDef.h)

