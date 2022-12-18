cmake_minimum_required(VERSION 3.25)
project(sandbox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
add_executable(sandbox main.cpp)