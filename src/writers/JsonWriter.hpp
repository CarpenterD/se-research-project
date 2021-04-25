#ifndef JSON_LOGGER
#define JSON_LOGGER

#include <ostream>
#include <string>
#include "ResultWriter.hpp"
#include "testing/TestResult.hpp"
#include "testing/SourceLocation.hpp"

class JsonWriter : public ResultWriter
{
    public:
        JsonWriter(std::ostream* stream, OutputConfig config);
        void OutputResults(std::vector<TestResult> results);

    private:
        OutputConfig config;
        std::ostream& output;

        void LogSingleResult(TestResult &result);
        std::string FormatSrcLoc(SourceLocation &loc);

        std::string DelimitString(std::string str);
};

#endif