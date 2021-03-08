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
        // higher level configuration containers
        RootConfig ParseRootConfig(tinyxml2::XMLElement* rootNode);
        OutputConfig ParseOutputConfig(tinyxml2::XMLElement* outputNode);
        TestConfig ParseTestConfig(tinyxml2::XMLElement* testNode);

        // Parse xml config elements into internal structure
        ConfigOption ParseConfigOption(tinyxml2::XMLElement* element);
        void ParseConfigOptions(tinyxml2::XMLElement* firstOption, std::vector<ConfigOption> &optionList);

        // helper function
        std::string GetElementName(tinyxml2::XMLElement* element);
    }
};

#endif