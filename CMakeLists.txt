cmake_minimum_required(VERSION 3.8)
project(metam)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(compiler-core)
add_subdirectory(metam-compiler-core)
add_subdirectory(metam-test)
