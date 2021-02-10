#ifndef MARKER_REGISRTY
#define MARKER_REGISTRY

#include <string>
#include <map>

#include "Marker.hpp"

class MarkerRegistry
{
    public:
        //must return a reference as Marker is an "interface"
        static Marker* CreateMarker(std::string markerName, std::string config);
};

#endif