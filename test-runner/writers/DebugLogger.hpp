#ifndef DEBUG_LOGGER
#define DEBUG_LOGGER

#include <string>
#include "ResultWriter.hpp"
#include "../testing/TestResult.hpp"
#include "../testing/SourceLocation.hpp"

class DebugLogger : public ResultWriter
{
    public:
        DebugLogger(std::string config);
        void OutputResults(std::vector<TestResult> results);

    private:
        std::string _config;

        void LogSingleResult(TestResult);
        std::string FormatSrcLoc(SourceLocation loc);
};

#endif