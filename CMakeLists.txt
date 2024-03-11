
cmake_minimum_required(VERSION 3.13)
project(runcJs C)
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ../bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(INCLUDE_DIR "includes")
set(SOURCE_DIR "src")
#
file(GLOB HEADERS  "${INCLUDE_DIR}/*.h")
file(GLOB SOURCES  "${SOURCE_DIR}/*.c", "${SOURCE_DIR}/**/*.c")
#
include_directories(${CMAKE_SOURCE_DIR}/includes/)
#
add_executable(program ${SOURCES})
target_include_directories(program PUBLIC "${INCLUDE_DIR}")
set_target_properties(program PROPERTIES C_FLAGS "-Wall")