cmake_minimum_required(VERSION 3.22)
project(patrick_standard_lib)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-elide-constructors")

add_executable(patrick_standard_lib main.cpp src/psl.h src/Converter.h src/rando/Messaging.h src/pub_sub/Manager.h src/pub_sub/Subscriber.h src/pub_sub/Publisher.h src/RingBuffer2.h src/rando/Function.h src/String.h src/Array.h src/RingBuff.h src/CsvParser.h
)



