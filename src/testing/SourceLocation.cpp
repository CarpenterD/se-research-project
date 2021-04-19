#include <string>
#include "SourceLocation.hpp"

// basic constructor
SourceLocation::SourceLocation(std::string file, int line, int column)
: file(file), line(line), column(column)
{

}
