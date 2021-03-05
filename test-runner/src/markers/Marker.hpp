#ifndef MARKER
#define MARKER

#include <string>
#include "testing/TestResult.hpp"
#include "utils/configuration/TestConfig.hpp"

class Marker
{
    public:
        virtual TestResult Mark(std::string file) = 0;
};

#endif