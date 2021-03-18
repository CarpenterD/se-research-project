#include <string>
#include "SourceLocation.hpp"

SourceLocation::SourceLocation(std::string file, int line, int column)
: file(file), line(line), column(column)
{
    snippet = std::string(); //empty string
    hasSnippet = false;
}

SourceLocation::SourceLocation(std::string file, int line, int column, std::string snippet)
: file(file), line(line), column(column), snippet(snippet), hasSnippet(true)
{

}
