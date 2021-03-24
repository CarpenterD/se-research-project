cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(stylemarker)

# Define paths and find llvm ---------------------------------------------
if(NOT DEFINED LLVM_ROOT)
  message(WARNING "LLVM_ROOT directory is not defined, attempting to use current directory")
  set(LLVM_ROOT .)
endif()

set(SRC_DIR src)

message(STATUS "Source dir '${SRC_DIR}'")
message(STATUS "LLVM root dir '${LLVM_ROOT}'")

set(LLVM_DIR ${LLVM_ROOT}/build/lib/cmake/llvm) # special variable used by find_package

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Project requirements ---------------------------------------------------
set(LIBRARY_LIST    clangAnalysis
                    clangAST
                    clangASTMatchers
                    clangBasic
                    clangDriver
                    clangEdit
                    clangFrontend
                    clangLex
                    clangParse
                    clangRewrite
                    clangRewriteFrontend
                    clangSema
                    clangSerialization
                    clangStaticAnalyzerFrontend
                    clangTooling
                    clangToolingCore)

# llvm
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Add source files and executable ----------------------------------------
file(GLOB SOURCES ${SRC_DIR}/*.cpp
                  ${SRC_DIR}/markers/*.cpp
                  ${SRC_DIR}/markers/options/*.cpp
                  ${SRC_DIR}/testing/*.cpp
                  ${SRC_DIR}/utils/*.cpp
                  ${SRC_DIR}/writers/*.cpp)

add_executable(stylemarker ${SOURCES})

# Include includes -------------------------------------------------------
# src
target_include_directories(stylemarker  PRIVATE ${SRC_DIR}
                                        PRIVATE ${LLVM_ROOT}/clang/include
                                        PRIVATE ${LLVM_ROOT}/build/tools/clang/include)

# and libraries...
target_link_libraries(stylemarker ${LIBRARY_LIST})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-strict-aliasing -fno-rtti -fuse-ld=lld")
target_link_libraries(stylemarker
    LLVMX86AsmParser # MC, MCParser, Support, X86Desc, X86Info
    LLVMX86Desc # MC, Support, X86AsmPrinter, X86Info
    LLVMX86Info # MC, Support, Target
    LLVMipo
    LLVMScalarOpts
    LLVMInstCombine
    LLVMTransformUtils
    LLVMAnalysis
    LLVMTarget
    LLVMOption # Support
    LLVMMCParser # MC, Support
    LLVMMC # Object, Support
    LLVMObject # BitReader, Core, Support
    LLVMBitReader # Core, Support
    LLVMCore # Support
    LLVMSupport
  )

# Build and link tinyXML library -----------------------------------------
set(TINY_XML_DIR src/tinyxml2)
add_library(tinyxml2 STATIC ${TINY_XML_DIR}/tinyxml2.cpp)
target_link_libraries(stylemarker tinyxml2)

#set build to debug mode -------------------------------------------------
target_compile_definitions(stylemarker PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")

# Print executable size --------------------------------------------------
add_custom_command(TARGET stylemarker POST_BUILD COMMAND ls -lh ./stylemarker)
