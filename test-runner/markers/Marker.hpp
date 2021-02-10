#ifndef MARKER
#define MARKER

#include <string>
#include "../testing/TestResult.hpp"

class Marker {
    public:
        virtual TestResult Mark(std::string file) = 0;
};

#endif