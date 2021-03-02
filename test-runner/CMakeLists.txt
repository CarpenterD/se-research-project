cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS  "-Wall")

project(stylemarker)

set(SRC_DIR src)

# Add source files
file(GLOB SOURCES ${SRC_DIR}/*.cpp
                  ${SRC_DIR}/markers/*.cpp
                  ${SRC_DIR}/testing/*.cpp
                  ${SRC_DIR}/utils/*.cpp
                  ${SRC_DIR}/writers/*.cpp)

add_executable(stylemarker ${SOURCES})
target_include_directories(stylemarker PRIVATE ${SRC_DIR})

#set build to debug mode
target_compile_definitions(stylemarker PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")

# Build and link tinyXML library
set(TINY_XML_DIR src/tinyxml2)
add_library(tinyxml2 STATIC ${TINY_XML_DIR}/tinyxml2.cpp)
target_link_libraries(stylemarker tinyxml2)
