#include <string>

#include "WriterRegistry.hpp"

#include "ResultWriter.hpp"
#include "DebugLogger.hpp"
#include "JsonWriter.hpp"


ResultWriter* WriterRegistry::CreateWriter(std::ostream* stream, OutputConfig config){
    
    // This implementation is not nearly complex enough for my liking
    std::string writerName = config.WriterName;
    if (writerName=="DebugLogger"){
        return new DebugLogger(stream, config);
    } else if (writerName=="JsonWriter"){
        return new JsonWriter(stream, config);
    } else {
        throw std::invalid_argument("Unable to construct writer '" + writerName + "'. Name not recognised.");
    }

}