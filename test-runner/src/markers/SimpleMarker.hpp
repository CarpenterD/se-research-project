#ifndef SIMPLE_MARKER
#define SIMPLE_MARKER

#include "Marker.hpp"
#include "utils/Configurations.hpp"

class SimpleMarker: public Marker
{
    public:
        SimpleMarker(TestConfig config);

        TestResult Mark(std::string file);
        std::string GetFeedback();
};

#endif