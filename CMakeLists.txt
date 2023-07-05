cmake_minimum_required(VERSION 3.10)

project(monads)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(monads monads.cpp)

target_compile_options(monads PRIVATE -g -fsanitize=address -Wall)
target_link_options(monads PRIVATE -g -fsanitize=address)
