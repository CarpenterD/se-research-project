# Thanks to John Fultz for this cmake template
## https://github.com/jfultz/libtooling_step_by_step


cmake_minimum_required (VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project (Samples)

add_executable(basic basic.cpp)
add_executable(indents_1 indents_1.cpp)
add_executable(hello_world hello_world.cpp)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
