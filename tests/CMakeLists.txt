cmake_minimum_required(VERSION 2.8.12) 
project(test)

include_directories(../include)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES
        ../source/Forward.cpp
        ../source/Inverted.cpp
        ../source/DocumentRepository.cpp
        ForwardIndexTests.cpp
        InvertedIndexTest.cpp
        DocumentRepositoryTest.cpp
        )

add_executable(tests ${SOURCE_FILES})
