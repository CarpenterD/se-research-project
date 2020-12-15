#!/bin/bash

llvmConfigPath=/path/to/llvmconfig
clangIncludePath=/your_llvm_location/clang/include

mkdir --verbose build samples_build

cd build
echo "Making main makefile..."
cmake .. -DCMAKE_PREFIX_PATH=$llvmConfigPath
cd ..

echo "Changing CPATH..."
echo "- old path: $CPATH"
export CPATH=$clangIncludePath
echo "- new path: $CPATH"

cd samples_build
echo "Making samples makefile..."
cmake ../samples
