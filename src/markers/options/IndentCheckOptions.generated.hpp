#ifndef OPTSET_INDENTCHECKOPTIONS
#define OPTSET_INDENTCHECKOPTIONS

#include <string>
#include <vector>
#include "utils/configuration/ConfigOption.hpp"

/**
Options used by the 'IndentationCheck' class.

Automatically generated config/options file.
*/
struct IndentCheckOptions
{
    int MaxInlineStmts; //!< The number of child stmts that can be inline with a parent
    const int DefaultMaxInlineStmts = 2; //!< default value for 'MaxInlineStmts' if no value is specified
    bool AllowStmtsOnSameLine; //!< Determines if multiple sibling statements should be allowed on the same line.
    const bool DefaultAllowStmtsOnSameLine = false; //!< default value for 'AllowStmtsOnSameLine' if no value is specified

    void Initialise(std::vector<ConfigOption> &options);
};

#endif