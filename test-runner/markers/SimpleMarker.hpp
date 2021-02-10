#ifndef SIMPLE_MARKER
#define SIMPLE_MARKER

#include "Marker.hpp"

class SimpleMarker: public Marker
{
    public:
        SimpleMarker(std::string config);

        TestResult Mark(std::string file);
        std::string GetFeedback();
};

#endif