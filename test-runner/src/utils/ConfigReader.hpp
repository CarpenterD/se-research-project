#ifndef CONFIG_READER
#define CONFIG_READER

#include "Configurations.hpp"

namespace ConfigReader
{
    RootConfig ReadRootConfigurations(std::string configFile);
};

#endif