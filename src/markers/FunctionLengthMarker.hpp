#ifndef FUNCTION_LENGTH_MARKER
#define FUNCTION_LENGTH_MARKER

#include "Marker.hpp"
#include "markers/options/CompilerOptions.generated.hpp"
#include "markers/options/FuncLenMkrOptions.generated.hpp"

class FunctionLengthMarker: public Marker, CompilerOptions, public FuncLenMkrOptions
{
    public:
        FunctionLengthMarker(TestConfig config);

        TestResult Mark(std::string file);
};

#endif