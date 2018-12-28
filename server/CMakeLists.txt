cmake_minimum_required(VERSION 3.13)
project(Retele)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(Retele main.cpp main.cpp)