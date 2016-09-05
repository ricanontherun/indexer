cmake_minimum_required(VERSION 3.5.1)
project(test)

include_directories(../include)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES
        ../source/Forward.cpp
        ForwardIndexTests.cpp)

add_executable(tests ${SOURCE_FILES})