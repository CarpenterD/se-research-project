#include "ConfigReader.hpp"

#include <string>
#include <stdexcept>
#include "FileIO.hpp"
#include "../tinyxml2/tinyxml2.h"

RootConfig ConfigReader::ReadConfigurations(std::string configFile)
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile(configFile.c_str());

    if (error != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "error: unable to open configuration file '" << configFile << "' -> " << doc.ErrorStr() << std::endl; 
        exit(EXIT_FAILURE);
    }

    tinyxml2::XMLElement* rootNode = doc.RootElement();
    if (std::string{rootNode->Name()} != "rootconfig")
    {
        //TODO Exceptions
        throw std::invalid_argument( "Configuration file format not recognised." );
    }

    return internal::ParseRootConfig(rootNode);
}

ConfigOption *ConfigReader::GetConfigOption(std::vector<ConfigOption> &options, std::string name, std::string optionSet)
{
    if (name == "")
    {
        throw std::invalid_argument("Unable to find config option; 'name' cannot be empty.");
    }

    ConfigOption *bestMatch = NULL;

    for (size_t i = 0; i < options.size(); i++)
    {
        if (options[i].Name != name)
        {
            continue;
        }
        // mark first match (but keep checking for a better one)
        if (bestMatch == NULL)
        {
            bestMatch = &options[i];
        }
        // return if perfect match found
        if (optionSet != "" && optionSet == options[i].OptionSet)
        {
            return &options[i];
        }
    }
    return bestMatch;
}

// INTERNAL METHODS -----------------------------

RootConfig ConfigReader::internal::ParseRootConfig(tinyxml2::XMLElement* rootNode)
{
    RootConfig config;

    if (rootNode->NoChildren() || rootNode->FirstChildElement("tests")==NULL || rootNode->FirstChildElement("output")==NULL)
    {
        throw std::invalid_argument( "Root configuration must contain at a collection of test configurations and an output configuration." );
    }

    // parse test configurations
    tinyxml2::XMLElement* testCollection = rootNode->FirstChildElement("tests");
    if (testCollection->NoChildren())
    {
        throw std::invalid_argument( "'tests' must contain at least one test configuration." );
    }

    tinyxml2::XMLElement* testConfig = testCollection->FirstChildElement("test");
    while (true)
    {
        config.Tests.push_back( ConfigReader::internal::ParseTestConfig(testConfig) );
        testConfig = testConfig->NextSiblingElement("test");
        if (testConfig==NULL)
        {
            break;
        }
    }

    // parse output configuration
    tinyxml2::XMLElement* outputConfig = rootNode->FirstChildElement("output");
    config.Ouput = ConfigReader::internal::ParseOutputConfig(outputConfig);

    return config;
}

TestConfig ConfigReader::internal::ParseTestConfig(tinyxml2::XMLElement* testNode)
{
    TestConfig config;

    //TODO check if config valid
    if (testNode == NULL)
    {
        throw std::invalid_argument( "Test element is null." );
    }

    config.TestName = GetElementName(testNode);
    ParseConfigOptions(testNode, config.Options);

    // manually add config option for test name so it can be parsed back later
    ConfigOption nameOption;
    nameOption.Name = "TestName";
    nameOption.Value = config.TestName;
    config.Options.push_back(nameOption);

    // parse std options into config
    ParseStdTestOptions(config);

    return config;
}

OutputConfig ConfigReader::internal::ParseOutputConfig(tinyxml2::XMLElement* outputNode)
{
    OutputConfig config;

    //TODO check if config valid
    if (outputNode == NULL)
    {
        throw std::invalid_argument( "Output element is null." );
    }

    config.WriterName = GetElementName(outputNode);
    
    return config;
}

// Parse xml configuration elements into internal struct

ConfigOption ConfigReader::internal::ParseConfigOption(tinyxml2::XMLElement* element)
{
    if (element == NULL)
    {
        throw std::runtime_error( "XML element is null. Unable to parse config option");
    }
    
    ConfigOption option;
    option.Name = GetElementName(element);

    // read optionSet if it is given
    const tinyxml2::XMLAttribute* osAttr = element->FindAttribute("optionSet");
    if (osAttr != NULL)
    {
        option.OptionSet = osAttr->Value();
    }

    // parse value(s)
    if (element->NoChildren() || element->FirstChildElement("value") == NULL)
    {
        throw std::runtime_error( "XML element on line "+ std::to_string(element->GetLineNum()) + " contains no 'value' child element. Unable to parse config option.");
    }
    for (tinyxml2::XMLElement* value = element->FirstChildElement("value"); value; value = value->NextSiblingElement("value"))
    {
        if (value->GetText() != NULL)
        {
            option.Values.push_back(value->GetText());
        }
        else
        {
            option.Values.push_back("");
        }
        option.Value = option.Values.back(); //set value to be the most recently (last) parsed value in the list
    }
    option.IsListProperty = option.Values.size() > 1;
    return option;
}

void ConfigReader::internal::ParseConfigOptions(tinyxml2::XMLElement* parent, std::vector<ConfigOption> &optionList)
{
    tinyxml2::XMLElement* element = parent->FirstChildElement("option");
    while (element != NULL)
    {
        try
        {
            optionList.push_back( ParseConfigOption(element) );
        }
        catch(const std::exception& e)
        {
            #ifdef DEBUG
            std::clog << "Exception thrown while parsing config option -> " << e.what() << std::endl;
            #endif
            // do nothing
        }
        element = element->NextSiblingElement();
    }
}

// HELPERS --------------------------------------

std::string ConfigReader::internal::GetElementName(tinyxml2::XMLElement* element)
{
    const tinyxml2::XMLAttribute* nameAttr = element->FindAttribute("name");
    if (nameAttr==NULL)
    {
        throw std::runtime_error( "Unable to retrieve 'name' attribute of XMLElement on line " + std::to_string(element->GetLineNum()));
    }
    return nameAttr->Value();
}