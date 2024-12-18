#
# Building C++ library for bragi-midi
#
cmake_minimum_required(VERSION 3.10)

project(bragi)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


### I. Setup bragi library
add_library(bragi
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bragi/midi/v1/message.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bragi/midi/v1/output.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bragi/midi/v1/note.cpp
)

target_include_directories(bragi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/)

target_link_libraries(bragi PRIVATE winmm)


### II. Compile examples
add_subdirectory(examples)


### III. PyBind11
