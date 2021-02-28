#ifndef STUB_MARKER
#define STUB_MARKER

#include "Marker.hpp"

class StubMarker: public Marker
{
    public:
        StubMarker(std::string config);

        TestResult Mark(std::string file);
};

#endif