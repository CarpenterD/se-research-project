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
{%- for opt in options %}
        {{opt.GetPropertyType()}} {{opt.GetPropertyName()}}; {{ "//!< "+opt.GetDocString() if opt.description }}
{%- endfor %}
    std::vector<ConfigOption> Options; //<! Other test options to be interpreted specifically by a test class using the options set they belong to.
};

#endif
