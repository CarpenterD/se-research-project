#include "FuncLenMkrOptions.generated.hpp"

#include <iostream>
#include "utils/ConfigReader.hpp"
#include "utils/ConfigConverter.hpp"

void FuncLenMkrOptions::Initialise(std::vector<ConfigOption> &options)
{
#ifdef DEBUG
    std::clog << "Initialising FuncLenMkrOptions" << std::endl;
#endif
    SuggestionThreshold = ConfigConverter::ConvertOption<int>(ConfigReader::GetConfigOption(options, "SuggestionThreshold", "FuncLenMkrOptions"), DefaultSuggestionThreshold);
    AcceptableThreshold = ConfigConverter::ConvertOption<int>(ConfigReader::GetConfigOption(options, "AcceptableThreshold", "FuncLenMkrOptions"), DefaultAcceptableThreshold);
}