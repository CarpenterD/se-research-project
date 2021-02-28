#include <string>

#include "MarkerRegistry.hpp"

#include "Marker.hpp"
#include "StubMarker.hpp"
#include "SimpleMarker.hpp"
#include "VowelCounter.hpp"

Marker* MarkerRegistry::CreateMarker(std::string markerName, std::string config){
    
    // This implementation is not nearly complex enough for my liking

    if (markerName=="StubMarker"){
        return new StubMarker(config);
    } else if (markerName=="SimpleMarker"){
        return new SimpleMarker(config);
    } else if (markerName=="VowelCounter"){
        return new VowelCounter(config);
    } else {
        throw std::invalid_argument("Unable to construct marker '" + markerName + "'. Name not recognised.");
    }
}