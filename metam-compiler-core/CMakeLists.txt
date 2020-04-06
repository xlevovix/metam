cmake_minimum_required(VERSION 3.8)
project(metam_compiler_core)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions($<$<CONFIG:DEBUG>:_DEBUG>)

add_library(${PROJECT_NAME} STATIC
  metam-comc.cpp
  metam-comc.hpp

  metam-comc/Framework.hpp
  metam-comc/Tokens.hpp
  metam-comc/Tokenizator.hpp
  metam-comc/ast.hpp)
include_directories("${CMAKE_SOURCE_DIR}/compiler-core")
target_link_libraries(${PROJECT_NAME} compiler_core)
