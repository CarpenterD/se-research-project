#ifndef SOURCE_LOCATION
#define SOURCE_LOCATION

#include <string>

struct SourceLocation
{
    SourceLocation(std::string file, int line, int column);
    SourceLocation(std::string file, int line, int column, std::string snippet);

    std::string fileName;
    int line;
    int column;
    std::string snippet;

    bool hasSnippet;
};

#endif