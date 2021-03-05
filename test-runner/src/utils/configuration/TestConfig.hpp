#ifndef TEST_CONFIG
#define TEST_CONFIG

#include <string>
#include <vector>
#include "utils/configuration/ConfigOption.hpp"

/**
 * Contains configurations and options for a specific test.
 * 
 * Automatically generated file.
*/
struct TestConfig
{
        std::string TestName; //!< The class name of the current test
        int MarksAvailable; //!< The total number of marks available for this test
    std::vector<ConfigOption> Options; //<! Other test options to be interpreted specifically by a test class using the options set they belong to.
};

#endif