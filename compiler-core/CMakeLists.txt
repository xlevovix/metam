cmake_minimum_required(VERSION 3.8)
project(compiler_core)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions($<$<CONFIG:DEBUG>:_DEBUG>)

add_library(${PROJECT_NAME} STATIC
  comc.cpp
  comc.hpp

  comc/Framework.hpp
  comc/Tokens.hpp
  comc/Lexer.hpp
  comc/Reporter.hpp
  comc/StreamLexer.hpp)
