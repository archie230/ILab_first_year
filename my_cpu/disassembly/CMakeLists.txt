cmake_minimum_required(VERSION 3.13)
project(disassembly)

set(CMAKE_CXX_STANDARD 17)

add_executable(disassembly main.cpp disasm.h disasm.cpp)
