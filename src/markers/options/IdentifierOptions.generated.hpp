#ifndef OPTSET_IDENTIFIEROPTIONS
#define OPTSET_IDENTIFIEROPTIONS

#include <string>
#include <vector>
#include "utils/configuration/ConfigOption.hpp"

/**
Options used by the 'IdentifierMarker' class.

Automatically generated config/options file.
*/
struct IdentifierOptions
{
    int MaxVarLength; //!< The maximum length of variable identifiers (in characters)
    const int DefaultMaxVarLength = 25; //!< default value for 'MaxVarLength' if no value is specified
    int MinFuncLength; //!< The minimum length of function identifiers (in characters)
    const int DefaultMinFuncLength = 3; //!< default value for 'MinFuncLength' if no value is specified
    int MaxFuncLength; //!< The maximum length of function identifiers (in characters)
    const int DefaultMaxFuncLength = 30; //!< default value for 'MaxFuncLength' if no value is specified
    std::vector<std::string> OneCharIdentifiers; //!< Valid one character long identifiers
    const std::vector<std::string> DefaultOneCharIdentifiers = {"c", "i", "j", "k", "n", "t", "x", "y"}; //!< default value for 'OneCharIdentifiers' if no value is specified

    void Initialise(std::vector<ConfigOption> &options);
};

#endif