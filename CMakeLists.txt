cmake_minimum_required(VERSION 3.10)

project(monad)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(monad monad.cpp)

target_compile_options(monad PRIVATE -g -fsanitize=address -Wall)
target_link_options(monad PRIVATE -g -fsanitize=address)
