#ifndef CONDITION_MARKER
#define CONDITION_MARKER

#include "Marker.hpp"
#include "markers/options/CompilerOptions.generated.hpp"

class ConditionMarker: public Marker, CompilerOptions
{
    public:
        ConditionMarker(TestConfig config);

        TestResult Mark(std::string file);
        std::string GetFeedback();

    private:
        TestResult result;
};

#endif