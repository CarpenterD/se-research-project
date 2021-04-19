#include <iostream>
#include <string>

#include "DebugLogger.hpp"
#include "testing/SourceLocation.hpp"
#include "testing/TestResult.hpp"

DebugLogger::DebugLogger(std::ostream* stream, OutputConfig config) : config(config), output(*stream){
#ifdef DEBUG
    std::cout << "Logger initialised (writing to log stream)" << std::endl;
#endif
}

std::string DebugLogger::FormatSrcLoc(SourceLocation loc){
    return loc.file + " " + std::to_string(loc.line) + ":" + std::to_string(loc.column);
}

std::string DebugLogger::FormatSrcRange(const SourceLocation &beg, const SourceLocation &end){
    return beg.file + " " + std::to_string(beg.line) + ":" + std::to_string(beg.column) + " -> " + std::to_string(end.line) + ":" + std::to_string(end.column);
}

void DebugLogger::LogSingleResult(TestResult result){
    output << "Marker: " << result.testName << std::endl;
    output << "Description: " << result.testDescription << std::endl;
    output << "Configuration Info: " << result.testInfo << std::endl;
    output << "Remarks: " << result.remarks << std::endl;
    output << "SCORE = " << result.marksAwarded << "/" << result.marksTotal << std::endl;
    output << "Feedback (" << result.feedback.size() << "): " << std::endl;
    // print feedback items
    for (size_t i = 0; i < result.feedback.size(); i++)
    {
        const SourceLocation &beg = result.feedback[i].start;

        if (!result.feedback[i].HasEndLocation()) {
            output << "- at " << FormatSrcLoc(beg);
        } else {
            output << "- at " << FormatSrcRange(beg, result.feedback[i].end);
        }

        output << std::endl;
        output << "\t" << result.feedback[i].comment << std::endl;
    }
}

void DebugLogger::OutputResults(std::vector<TestResult> results){
    output << "----- RESULTS -----" << std::endl;
    for (size_t i = 0; i < results.size(); i++)
    {
        output << "[Test " << i+1 << "/" << results.size() << "]" << std::endl;
        LogSingleResult(results[i]);
        output << std::endl;
    }
}