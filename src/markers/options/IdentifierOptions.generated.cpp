#include "IdentifierOptions.generated.hpp"

#include <iostream>
#include "utils/ConfigReader.hpp"
#include "utils/ConfigConverter.hpp"

void IdentifierOptions::Initialise(std::vector<ConfigOption> &options)
{
#ifdef DEBUG
    std::clog << "Initialising IdentifierOptions" << std::endl;
#endif
    MaxVarLength = ConfigConverter::ConvertOption<int>(ConfigReader::GetConfigOption(options, "MaxVarLength", "IdentifierOptions"), DefaultMaxVarLength);
    MinFuncLength = ConfigConverter::ConvertOption<int>(ConfigReader::GetConfigOption(options, "MinFuncLength", "IdentifierOptions"), DefaultMinFuncLength);
    MaxFuncLength = ConfigConverter::ConvertOption<int>(ConfigReader::GetConfigOption(options, "MaxFuncLength", "IdentifierOptions"), DefaultMaxFuncLength);
    ConfigConverter::ConvertOption<std::string>(ConfigReader::GetConfigOption(options, "OneCharIdentifiers", "IdentifierOptions"), OneCharIdentifiers, DefaultOneCharIdentifiers);
}