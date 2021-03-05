#ifndef OPTSET_SIMPLEOPTIONS
#define OPTSET_SIMPLEOPTIONS

#include <string>

/**
A collection of simple options for testing.

Automatically generated config/options file.
*/
class SimpleOptions
{
    private:
        std::string feedbackTone; //!< The tone/colour of feedback generated by the test
        const std::string defaultFeedbackTone = "blunt"; //!< default value for 'feedbackTone' if no value is specified
        int bonusMarks; //!< The number of bonus marks to assign
        const int defaultBonusMarks = 0; //!< default value for 'bonusMarks' if no value is specified
};

#endif