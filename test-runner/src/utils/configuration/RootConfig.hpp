#ifndef ROOT_CONFIG
#define ROOT_CONFIG

#include <vector>
#include "utils/configuration/TestConfig.hpp"
#include "utils/configuration/OutputConfig.hpp"

struct RootConfig
{
    // Other standard configurations go here
    std::vector<TestConfig> Tests;
    OutputConfig Ouput;
};

#endif