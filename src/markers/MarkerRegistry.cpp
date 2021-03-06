#include <string>

#include "MarkerRegistry.hpp"

#include "Marker.hpp"
#include "StubMarker.hpp"
#include "SimpleMarker.hpp"
#include "VowelCounter.hpp"
#include "FunctionLengthMarker.hpp"
#include "IndentationCheck.hpp"
#include "ConditionMarker.hpp"
#include "IdentifierMarker.hpp"
#include "FailingMarker.hpp"

Marker* MarkerRegistry::CreateMarker(TestConfig config)
{    
    // This implementation is not nearly complex enough for my liking
    std::string markerName = config.TestName;
    if (markerName=="StubMarker"){
        return new StubMarker(config);
    } else if (markerName=="SimpleMarker"){
        return new SimpleMarker(config);
    } else if (markerName=="FailingMarker"){
        return new FailingMarker(config);
    } else if (markerName=="FunctionLengthMarker"){
        return new FunctionLengthMarker(config);
    } else if (markerName=="IndentationCheck"){
        return new IndentationCheck(config);
    } else if (markerName=="ConditionMarker"){
        return new ConditionMarker(config);
    } else if (markerName=="VowelCounter"){
        return new VowelCounter(config);
    } else if (markerName=="IdentifierMarker"){
        return new IdentifierMarker(config);
    } else {
        throw std::invalid_argument("Unable to construct marker '" + markerName + "'. Name not recognised.");
    }
}