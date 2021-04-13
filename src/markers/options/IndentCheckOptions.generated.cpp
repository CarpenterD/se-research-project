#include "IndentCheckOptions.generated.hpp"

#include <iostream>
#include "utils/ConfigReader.hpp"
#include "utils/ConfigConverter.hpp"

void IndentCheckOptions::Initialise(std::vector<ConfigOption> &options)
{
#ifdef DEBUG
    std::clog << "Initialising IndentCheckOptions" << std::endl;
#endif
    MaxInlineStmts = ConfigConverter::ConvertOption<int>(ConfigReader::GetConfigOption(options, "MaxInlineStmts", "IndentCheckOptions"), DefaultMaxInlineStmts);
    AllowStmtsOnSameLine = ConfigConverter::ConvertOption<bool>(ConfigReader::GetConfigOption(options, "AllowStmtsOnSameLine", "IndentCheckOptions"), DefaultAllowStmtsOnSameLine);
}