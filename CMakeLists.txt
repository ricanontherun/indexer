cmake_minimum_required(VERSION 3.6)
project(indexer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(include)

set(SOURCE_FILES main.cpp source/Forward.cpp)
add_executable(indexer ${SOURCE_FILES})