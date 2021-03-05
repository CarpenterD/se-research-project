#ifndef CONFIG_READER
#define CONFIG_READER

#include "utils/configuration/RootConfig.hpp"
#include "tinyxml2/tinyxml2.h"

namespace ConfigReader
{
    RootConfig ReadConfigurations(std::string configFile);

    // internal namespace to to disuade external use
    namespace internal
    {
        RootConfig ParseRootConfig(tinyxml2::XMLElement* rootNode);
        OutputConfig ParseOutputConfig(tinyxml2::XMLElement* outputNode);
        TestConfig ParseTestConfig(tinyxml2::XMLElement* testNode);
    }
};

#endif