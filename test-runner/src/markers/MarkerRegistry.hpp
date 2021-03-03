#ifndef MARKER_REGISRTY
#define MARKER_REGISTRY

#include <string>
#include <map>

#include "Marker.hpp"
#include "utils/Configurations.hpp"

class MarkerRegistry
{
    public:
        //must return a reference as Marker is an "interface"
        static Marker* CreateMarker(TestConfig config);
};

#endif