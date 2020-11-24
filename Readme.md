# Software Engineering Research Project
Final research project for the Master of Professional Engineering (Software Engineering) at the University of Western Australia, supervised by Chris McDonald.

This will eventuially become an automated code-style analysis/marking tool used for marking second-year programming projects written in C.

Makes use of the [clang](https://clang.llvm.org/) compiler frontend.

### To set up and run cmake
1. Create and change to a directory called `build`:
```bash
    mkdir build
    cd build
```
2. Run cmake to generate the makefile:
```bash
    cmake .. -DCMAKE_PREFIX_PATH=/path/to/llvm
```
  - Where `/path/to/llvm` is the directory containing `llvm-config`.

  - You also need to add the location of clang headers to your `CPATH` variable. This can be done using:

```bash
    export CPATH=/your_llvm_location/clang/include
```
3. Return to the main directory. Create and change to a directory called `samples_build`:
```bash
    cd ..
    mkdir samples_build
    cd samples_build
```
4. Run cmake to generate the makefile:
```bash
    cmake ../samples
```
5. Return to main directory.
```bash
    cd ..
```

If you are adding code samples or other code files, don't forget to specify them in the relevant cmake file.

### To build programs and code samples
Code samples can be built with `make` in the `samples_build` directory.

Main system code can be built with `make` in the `build` directory.

### To run the code
Run the code from the `build` directory with the following command:
```
    ./<executable_name> -p=../samples_build <target_file>
```
Note that the `-p=../samples_build` option is only used for establishing the ClangTool context and can be ommitted.

### Useful things
- [clang doxygen docs](https://clang.llvm.org/doxygen/index.html)
- This repo was adapted from [John Fultz's Tutorial](https://github.com/jfultz/libtooling_step_by_step)
- To do an AST dump, run:

````
    clang++ -Xclang -ast-dump -fsyntax-only filename.cpp
````
