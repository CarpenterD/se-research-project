#ifndef TEST_RESULT
#define TEST_RESULT

#include <vector>
#include "testing/FeedbackItem.hpp"

struct TestResult
{
    std::string testName;
    std::string testDescription; // description about the *purpose* of the test
    std::string testInfo; // information about *how* the test is configured
    std::string remarks; // summative remarks about *what* was tested

    int marksTotal;
    int marksAwarded;
    std::vector<FeedbackItem> feedback;
};

#endif