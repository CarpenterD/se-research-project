#ifndef SOURCE_LOCATION
#define SOURCE_LOCATION

#include <string>

struct SourceLocation
{
    SourceLocation(std::string fileName, int lineNum, int columnNum);

    std::string file;
    int line;
    int column;
};

#endif
