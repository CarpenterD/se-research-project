#include <iostream>
#include <string>

#include "DebugLogger.hpp"
#include "../testing/SourceLocation.hpp"
#include "../testing/TestResult.hpp"

DebugLogger::DebugLogger(std::string config){
    _config = config;
    std::clog << "Logger initialised (writing to log stream). Config:\n" << _config << std::endl;
}

std::string DebugLogger::FormatSrcLoc(SourceLocation loc){
    std::cout << "FSL" << std::endl;
    return loc.GetFileName() + " " + std::to_string(loc.GetLineNumber()) + ":" + std::to_string(loc.GetColumnNumber());
}

void DebugLogger::LogSingleResult(TestResult result){
    std::cout << "DebugLogger SINGLE" << std::endl;
    std::clog << "Marker: " << result.testName << std::endl;
    std::clog << "Description: " << result.testDescription << std::endl;
    std::clog << "Configuration Info: " << result.testInfo << std::endl;
    std::clog << "Remarks: " << result.remarks << std::endl;
    std::clog << "SCORE = " << result.marksAwarded << "/" << result.marksTotal << std::endl;
    std::clog << "Feedback (" << result.feedback.size() << "): " << std::endl;
    // print feedback items
    for (size_t i = 0; i < result.feedback.size(); i++)
    {
        SourceLocation loc = result.feedback[i].first;
        std::string comment = result.feedback[i].second;
        std::clog << "- at " << FormatSrcLoc(loc);
        if (loc.hasSnippet){
            std::clog << " - \"" << loc.GetCodeSnippet() << "\"";
        }
        std::clog << std::endl;
        std::clog << "\t" << comment << std::endl;
    }
}

void DebugLogger::OutputResults(std::vector<TestResult> results){
    std::cout << "DebugLogger" << std::endl;
    for (size_t i = 0; i < results.size(); i++)
    {
        std::clog << "[Test " << i+1 << "/" << results.size() << "]" << std::endl;
        LogSingleResult(results[i]);
        std::clog << std::endl;
    }
}