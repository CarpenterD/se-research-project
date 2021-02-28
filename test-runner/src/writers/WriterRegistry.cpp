#include <string>

#include "WriterRegistry.hpp"

#include "ResultWriter.hpp"
#include "DebugLogger.hpp"


ResultWriter* WriterRegistry::CreateWriter(std::string writerName, std::ostream* stream, std::string config){
    
    // This implementation is not nearly complex enough for my liking
    
    if (writerName=="DebugLogger"){
        return new DebugLogger(stream, config);
    } else {
        return nullptr;
    }

}