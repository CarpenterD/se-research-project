#ifndef WRITER_REGISRTY
#define WRITER_REGISRTY

#include <ostream>
#include <string>
#include <map>

#include "ResultWriter.hpp"

class WriterRegistry
{
    public:
        //must return a reference as ResultWriter is an "interface"
        static ResultWriter* CreateWriter(std::string writerName, std::ostream* stream, std::string config);
};

#endif