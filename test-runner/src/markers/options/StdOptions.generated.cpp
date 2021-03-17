#include "StdOptions.generated.hpp"

#include <iostream>
#include "utils/ConfigReader.hpp"
#include "utils/ConfigConverter.hpp"

void StdOptions::Initialise(std::vector<ConfigOption> &options)
{
#ifdef DEBUG
    std::clog << "Initialising StdOptions" << std::endl;
#endif
    TestName = ConfigConverter::ConvertOption<std::string>(ConfigReader::GetConfigOption(options, "TestName", "StdOptions"));
    MarksAvailable = ConfigConverter::ConvertOption<int>(ConfigReader::GetConfigOption(options, "MarksAvailable", "StdOptions"));
}