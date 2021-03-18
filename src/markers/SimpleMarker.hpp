#ifndef SIMPLE_MARKER
#define SIMPLE_MARKER

#include "Marker.hpp"
#include "markers/options/SimpleOptions.generated.hpp"

class SimpleMarker: public Marker, public SimpleOptions
{
    public:
        SimpleMarker(TestConfig config);

        TestResult Mark(std::string file);
        std::string GetFeedback();
};

#endif