#ifndef OPTSET_STDOPTIONS
#define OPTSET_STDOPTIONS

#include <string>
#include <vector>
#include "utils/configuration/ConfigOption.hpp"

/**
Automatically generated config/options file.
*/
struct StdOptions
{
    std::string TestName; //!< The class name of the current test
    int MarksAvailable; //!< The total number of marks available for this test

    void Initialise(std::vector<ConfigOption> &options);
};

#endif