#include "CompilerOptions.generated.hpp"

#include <iostream>
#include "utils/ConfigReader.hpp"
#include "utils/ConfigConverter.hpp"

void CompilerOptions::Initialise(std::vector<ConfigOption> &options)
{
#ifdef DEBUG
    std::clog << "Initialising CompilerOptions" << std::endl;
#endif
    ConfigConverter::ConvertOption<std::string>(ConfigReader::GetConfigOption(options, "Flags", "CompilerOptions"), Flags, DefaultFlags);
}