#include <iostream>
#include <string>

#include "DebugLogger.hpp"
#include "../testing/SourceLocation.hpp"
#include "../testing/TestResult.hpp"

DebugLogger::DebugLogger(std::ostream* stream, std::string config) : config(config), output(*stream){
#ifdef DEBUG
    std::cout << "Logger initialised (writing to log stream). Config:\n" << config << std::endl;
#endif
}

std::string DebugLogger::FormatSrcLoc(SourceLocation loc){
    return loc.file + " " + std::to_string(loc.line) + ":" + std::to_string(loc.column);
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
        SourceLocation loc = result.feedback[i].first;
        std::string comment = result.feedback[i].second;
        output << "- at " << FormatSrcLoc(loc);
        if (loc.hasSnippet){
            output << " - \"" << loc.snippet << "\"";
        }
        output << std::endl;
        output << "\t" << comment << std::endl;
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