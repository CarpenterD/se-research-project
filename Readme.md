# Software Engineering Research Project
Final research project for the Master of Professional Engineering (Software Engineering) at the University of Western Australia, supervised by Chris McDonald.

This will eventuially become an automated code-style analysis/marking tool used for marking second-year programming projects written in C.

Makes use of the [clang](https://clang.llvm.org/) compiler frontend.

### To set up and run cmake
1. Replace the path locations in the setup script and run it in the current shell:
```bash
    source setup.sh
```

If you are adding code samples or other code files, don't forget to specify them in the relevant cmake file.

Use `-DCMAKE_BUILD_TYPE=[Release/Debug]` to turn on or off additional debugging information.

### To build programs and code samples
Main system code can be built with `make` in the `build` directory.

### To run the code
Run the code from the `build` directory with the following command:
```
    ./stylemarker <target_file>
```

### Useful things
- [clang doxygen docs](https://clang.llvm.org/doxygen/index.html)
- This repo was adapted from [John Fultz's Tutorial](https://github.com/jfultz/libtooling_step_by_step)
- To do an AST dump, run:

````
    clang++ -Xclang -ast-dump -fsyntax-only filename.cpp
````
