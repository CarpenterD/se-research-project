#ifndef MARKER
#define MARKER

#include <string>
#include "testing/TestResult.hpp"
#include "markers/options/StdOptions.generated.hpp"
#include "utils/configuration/TestConfig.hpp"

class Marker : public StdOptions
{
    public:
        virtual TestResult Mark(std::string file) = 0;
};

#endif