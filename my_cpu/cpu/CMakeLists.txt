cmake_minimum_required(VERSION 3.13)
project(cpu)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpu main.cpp cpu.h cpu.cpp)