#ifndef SOURCE_LOCATION
#define SOURCE_LOCATION

#include <string>

class SourceLocation
{
    public:
        // constructors
        SourceLocation(std::string file, int line, int column);
        SourceLocation(std::string file, int line, int column, std::string snippet);

        // utilities
        bool hasSnippet;

        // get-ers
        std::string GetFileName();
        int GetLineNumber();
        int GetColumnNumber();
        std::string GetCodeSnippet();

    private:
        std::string _fileName;
        int _line;
        int _column;
        std::string _snippet;
};

#endif