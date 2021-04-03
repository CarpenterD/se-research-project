#ifndef OPTSET_COMPILEROPTIONS
#define OPTSET_COMPILEROPTIONS

#include <string>
#include <vector>
#include "utils/configuration/ConfigOption.hpp"

/**
Options for compiling source code and specifying the behaviour of the clang compiler.

Automatically generated config/options file.
*/
struct CompilerOptions
{
    std::vector<std::string> Flags; //!< Flags to pass on to compiler
    const std::vector<std::string> DefaultFlags = {"-std=c99", "-Wall", "-Werror", "-pedantic"}; //!< default value for 'Flags' if no value is specified

    void Initialise(std::vector<ConfigOption> &options);
};

#endif