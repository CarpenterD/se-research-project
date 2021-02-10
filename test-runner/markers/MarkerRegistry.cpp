#include <string>

#include "MarkerRegistry.hpp"

#include "Marker.hpp"
#include "StubMarker.hpp"
#include "SimpleMarker.hpp"

Marker* MarkerRegistry::CreateMarker(std::string markerName, std::string config){
    
    // This implementation is not nearly complex enough for my liking
    
    if (markerName=="StubMarker"){
        return new StubMarker(config);
    } else if (markerName=="SimpleMarker"){
        return new SimpleMarker(config);
    } else {
        return nullptr;
    }

}