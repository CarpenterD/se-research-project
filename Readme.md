# Software Engineering Research Project
Final research project for the Master of Professional Engineering (Software Engineering) at the University of Western Australia, supervised by Chris McDonald.

This will eventuially become an automated code-style analysis/marking tool used for marking second-year programming projects written in C.

Makes use of the [clang](https://clang.llvm.org/) compiler frontend.

### To set up and run cmake
Create a directory `build`, change to it and call `cmake` on the parent directory.
```bash
    mkdir build
    cd build
    cmake .. -DLLVM_ROOT=<YourLLVMLocation>
```
*Note* - you must first [clone and build the llvm source repository](https://clang.llvm.org/get_started.html "Getting Started with Clang"). `<YourLLVMLocation>` is the root directory of that repository.

If you are adding code samples or other code files, don't forget to specify them in `CMakeLists.txt`.


You may also use `-DCMAKE_BUILD_TYPE=[Release/Debug]` to turn on or off additional debugging information.

### To build the main program
Main system code can be built with `make` from the `build` directory.
```bash
    make
```

### To run the code
Run the code from the `build` directory with the following command:
```
    ./stylemarker <target_file>
```
(or use the `--help` option to display usage)

### Useful things
- [clang doxygen docs](https://clang.llvm.org/doxygen/index.html)
- This repo was adapted from [John Fultz's Tutorial](https://github.com/jfultz/libtooling_step_by_step)
- To do an AST dump, run:

````
    clang++ -Xclang -ast-dump -fsyntax-only filename.cpp
````
