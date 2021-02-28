#ifndef DEBUG_LOGGER
#define DEBUG_LOGGER

#include <ostream>
#include <string>
#include "ResultWriter.hpp"
#include "../testing/TestResult.hpp"
#include "../testing/SourceLocation.hpp"

class DebugLogger : public ResultWriter
{
    public:
        DebugLogger(std::ostream* stream, std::string config);
        void OutputResults(std::vector<TestResult> results);

    private:
        std::string config;
        std::ostream& output;

        void LogSingleResult(TestResult);
        std::string FormatSrcLoc(SourceLocation loc);
};

#endif