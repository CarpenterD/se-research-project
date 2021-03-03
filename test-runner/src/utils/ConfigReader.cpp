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

// internal methods
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

    // parse test name
    const tinyxml2::XMLAttribute* nameAttr = testNode->FindAttribute("name");
    if (nameAttr==NULL)
    {
        throw std::invalid_argument( "Test configuration on line " + std::to_string(testNode->GetLineNum()) + " has no 'name' attribute." );
    }
    config.TestName = nameAttr->Value();

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

    // parse test name
    const tinyxml2::XMLAttribute* nameAttr = outputNode->FindAttribute("name");
    if (nameAttr==NULL)
    {
        throw std::invalid_argument( "Output configuration on line " + std::to_string(outputNode->GetLineNum()) + " has no 'name' attribute." );
    }
    config.WriterName = nameAttr->Value();
    
    return config;
}
