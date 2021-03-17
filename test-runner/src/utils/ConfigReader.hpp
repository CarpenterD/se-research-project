#ifndef CONFIG_READER
#define CONFIG_READER

#include "utils/configuration/RootConfig.hpp"
#include "tinyxml2/tinyxml2.h"

namespace ConfigReader
{
    RootConfig ReadConfigurations(std::string configFile);

    /**
     * Returns a ConfigOption from a collection, matched by name and parent option set.
     * If multiple matches are found the first match is returned, with a preference given to a match with both name and option set.
    */
    ConfigOption *GetConfigOption(std::vector<ConfigOption> &options, std::string name, std::string optionSet = "");

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
        void ParseStdTestOptions(TestConfig &config);

        // helper function
        std::string GetElementName(tinyxml2::XMLElement* element);
    }
};

#endif