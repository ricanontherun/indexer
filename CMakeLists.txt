cmake_minimum_required(VERSION 2.8.12)
project(indexer)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

include_directories(include)

set(SOURCE_FILES main.cpp source/Forward.cpp)
add_executable(indexer ${SOURCE_FILES})
