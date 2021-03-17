#include "SimpleOptions.generated.hpp"

#include <iostream>
#include "utils/ConfigReader.hpp"
#include "utils/ConfigConverter.hpp"

void SimpleOptions::Initialise(std::vector<ConfigOption> &options)
{
#ifdef DEBUG
    std::clog << "Initialising SimpleOptions" << std::endl;
#endif
    FeedbackMessage = ConfigConverter::ConvertOption<std::string>(ConfigReader::GetConfigOption(options, "FeedbackMessage", "SimpleOptions"), DefaultFeedbackMessage);
    BonusMarks = ConfigConverter::ConvertOption<int>(ConfigReader::GetConfigOption(options, "BonusMarks", "SimpleOptions"), DefaultBonusMarks);
}