#ifndef WRITER_REGISRTY
#define WRITER_REGISRTY

#include <ostream>
#include <string>
#include <map>

#include "ResultWriter.hpp"
#include "utils/Configurations.hpp"

class WriterRegistry
{
    public:
        //must return a reference as ResultWriter is an "interface"
        static ResultWriter* CreateWriter(std::ostream* stream, OutputConfig config);
};

#endif