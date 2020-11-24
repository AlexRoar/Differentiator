cmake_minimum_required(VERSION 3.17)
project(Differentiator)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

set(CMAKE_CXX_STANDARD 20)

include_directories(src/)
add_executable(Differentiator Examples/main.cpp)