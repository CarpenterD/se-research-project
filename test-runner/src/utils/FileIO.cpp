#include "FileIO.hpp"

#include <stdexcept>

std::istream* CreateInputStream(std::string fileName){
#ifdef DEBUG
    std::cout << "Opening input stream from file '" << fileName << "'" << std::endl;
#endif

    if (fileName == "") {
        throw std::invalid_argument( "Unable to read from an empty filepath" );
    }

    std::ifstream* ifs = new std::ifstream;
    ifs->open(fileName, std::ios::in);
    if (ifs->is_open()) {
        return ifs;
    }
    throw std::runtime_error("unable to open an input stream from '"+fileName+"'");
}

std::ostream* CreateOutputStream(std::string fileName){
#ifdef DEBUG
    std::cout << "Opening output stream to file '" << fileName << "'" << std::endl;
#endif

    if (fileName == "") {
        return &std::cout;
    }
    
    std::ofstream* ofs = new std::ofstream;
    ofs->open(fileName, std::ios::out | std::ios::trunc);
    if (ofs->is_open()) {
        return ofs;
    }
    throw std::runtime_error("unable to open an output stream to '"+fileName+"'");
}