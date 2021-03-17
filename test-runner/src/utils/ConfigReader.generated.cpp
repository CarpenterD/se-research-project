#include "ConfigReader.hpp"

#include <string>
#include "ConfigConverter.hpp"

void ConfigReader::internal::ParseStdTestOptions(TestConfig &config)
{
    config.TestName = ConfigConverter::ConvertOption<std::string>(ConfigReader::GetConfigOption(config.Options, "TestName", "StdOptions"));
    config.MarksAvailable = ConfigConverter::ConvertOption<int>(ConfigReader::GetConfigOption(config.Options, "MarksAvailable", "StdOptions"));
}
