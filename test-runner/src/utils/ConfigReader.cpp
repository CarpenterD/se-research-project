#include "ConfigReader.hpp"

#include "FileIO.hpp"
#include "../tinyxml2/tinyxml2.h"

RootConfig ConfigReader::ReadRootConfigurations(std::string configFile){
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile(configFile.c_str());

    if (error != tinyxml2::XML_SUCCESS){
        //TODO
        std::cerr << "error: unable to open configuration file '" << configFile << "' -> " << doc.ErrorStr() << std::endl; 
        exit(EXIT_FAILURE);
    }
    RootConfig root;
    return root;
}