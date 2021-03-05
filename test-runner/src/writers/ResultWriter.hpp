#ifndef RESULT_WRITER
#define RESULT_WRITER

#include <vector>
#include "testing/TestResult.hpp"
#include "utils/configuration/OutputConfig.hpp"

class ResultWriter
{
    public:
        virtual void OutputResults(std::vector<TestResult> results) = 0;
};

#endif