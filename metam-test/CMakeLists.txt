cmake_minimum_required(VERSION 3.8)
project(metam_test)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions($<$<CONFIG:DEBUG>:_DEBUG>)

add_executable(${PROJECT_NAME}
  main.cpp
  )
include_directories("${CMAKE_SOURCE_DIR}/compiler-core")
include_directories("${CMAKE_SOURCE_DIR}/metam-compiler-core")
target_link_libraries(${PROJECT_NAME} compiler_core)
target_link_libraries(${PROJECT_NAME} metam_compiler_core)

configure_file(${CMAKE_SOURCE_DIR}/examples/0-hello-world.metm ${CMAKE_CURRENT_BINARY_DIR}/file.metm COPYONLY)
