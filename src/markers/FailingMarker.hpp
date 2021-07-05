#ifndef FAILING_MARKER
#define FAILING_MARKER

#include "Marker.hpp"

class FailingMarker: public Marker
{
    public:
        FailingMarker(TestConfig config);

        TestResult Mark(std::string file);
};

#endif