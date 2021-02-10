#include <string>
#include "SourceLocation.hpp"

SourceLocation::SourceLocation(std::string file, int line, int column){
    _fileName = file;
    _line = line;
    _column = column;
    _snippet = std::string(); //empty string
    hasSnippet = false;
}

SourceLocation::SourceLocation(std::string file, int line, int column, std::string snippet){
    _fileName = file;
    _line = line;
    _column = column;
    _snippet = snippet;
    hasSnippet = true;
}

std::string SourceLocation::GetFileName(){
    return _fileName;
}

int SourceLocation::GetLineNumber(){
    return _line;
}

int SourceLocation::GetColumnNumber(){
    return _column;
}

std::string SourceLocation::GetCodeSnippet(){
    return _snippet;
}