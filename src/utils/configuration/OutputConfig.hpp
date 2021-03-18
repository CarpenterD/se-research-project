#ifndef OUTPUT_CONFIG
#define OUTPUT_CONFIG

#include <string>
#include <vector>
#include "utils/configuration/ConfigOption.hpp"

struct OutputConfig
{
    std::string WriterName;
    std::vector<ConfigOption> Options; 
};

#endif