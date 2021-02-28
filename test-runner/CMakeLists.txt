cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS  "-Wall")

project(stylemarker)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(stylemarker ${SOURCES})

target_compile_definitions(stylemarker PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
target_include_directories(stylemarker PRIVATE src)