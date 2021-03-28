#ifndef OPTSET_FUNCLENMKROPTIONS
#define OPTSET_FUNCLENMKROPTIONS

#include <string>
#include <vector>
#include "utils/configuration/ConfigOption.hpp"

/**
Options used by the 'FunctionLengthMarker' class.

Automatically generated config/options file.
*/
struct FuncLenMkrOptions
{
    int SuggestionThreshold; //!< The threshold for function length above which the program should make a suggestion
    const int DefaultSuggestionThreshold = 40; //!< default value for 'SuggestionThreshold' if no value is specified
    int AcceptableThreshold; //!< The upper threshold for acceptable function length. Penalties issued for exceeding this length.
    const int DefaultAcceptableThreshold = 60; //!< default value for 'AcceptableThreshold' if no value is specified

    void Initialise(std::vector<ConfigOption> &options);
};

#endif