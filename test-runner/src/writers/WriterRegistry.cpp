#include <string>

#include "WriterRegistry.hpp"

#include "ResultWriter.hpp"
#include "DebugLogger.hpp"


ResultWriter* WriterRegistry::CreateWriter(std::ostream* stream, OutputConfig config){
    
    // This implementation is not nearly complex enough for my liking
    std::string writerName = config.WriterName;
    if (writerName=="DebugLogger"){
        return new DebugLogger(stream, config);
    } else {
        throw std::invalid_argument("Unable to construct writer '" + writerName + "'. Name not recognised.");
    }

}