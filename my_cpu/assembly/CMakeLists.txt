cmake_minimum_required(VERSION 3.10)
project(assembly)

set(CMAKE_CXX_STANDARD 17)

add_executable(assembly main.cpp assembly.h assembly.cpp maps.h maps.cpp
                                        string_to_int.h string_to_int.cpp tokens.h tokens.cpp)
