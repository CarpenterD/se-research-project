#include <iostream>
#include <string>
#include <regex>

#include "JsonWriter.hpp"
#include "testing/TestResult.hpp"

JsonWriter::JsonWriter(std::ostream* stream, OutputConfig config) : config(config), output(*stream){
#ifdef DEBUG
    std::cout << "JSON Writer initialised" << std::endl;
#endif
}

std::string JsonWriter::FormatSrcLoc(SourceLocation &loc) {
    return "{\"file\": \"" + loc.file + "\", \"line\": " + std::to_string(loc.line) + ", \"column\": " + std::to_string(loc.column) + "}";
}

std::string JsonWriter::DelimitString(std::string str) {
    std::regex pattern ("\"");
    return std::regex_replace(str, pattern,"\\\""); // replace " with \"
}

void JsonWriter::LogSingleResult(TestResult &result){
    output << "\t\t\t\"marker\": \""      << result.testName        << "\"," << std::endl;
    output << "\t\t\t\"description\": \"" << result.testDescription << "\"," << std::endl;
    output << "\t\t\t\"configInfo\": \""  << result.testInfo        << "\"," << std::endl;
    output << "\t\t\t\"remarks\": \""     << result.remarks         << "\"," << std::endl;
    
    double score = result.marksTotal == 0? 0 : result.marksAwarded / (double) result.marksTotal;
    
    output << "\t\t\t\"score\": "       << score               << "," << std::endl;
    output << "\t\t\t\"marksAwarded\": "<< result.marksAwarded << "," << std::endl;
    output << "\t\t\t\"marksTotal\": "  << result.marksTotal   << "," << std::endl;

    output << "\t\t\t\"feedback\": [" << std::endl;
    // print feedback items
    for (size_t i = 0; i < result.feedback.size(); i++)
    {
        output << "\t\t\t\t{" << std::endl;

        SourceLocation &beg = result.feedback[i].start;
        output << "\t\t\t\t\t\"start\": " << FormatSrcLoc(beg) << "," << std::endl;

        if (result.feedback[i].HasEndLocation()) {
            output << "\t\t\t\t\t\"end\": " << FormatSrcLoc(result.feedback[i].end) << "," << std::endl;
        }

        output << "\t\t\t\t\t\"comment\": \"" << DelimitString(result.feedback[i].comment) << "\"" << std::endl;
        output << "\t\t\t\t}" << (i == result.feedback.size()-1? "" : ",") << std::endl;
    }
    output << (result.feedback.size()==0?"":"\t\t\t") << "]" << std::endl;
}

void JsonWriter::OutputResults(std::vector<TestResult> results){
    output << "{\n\t\"results\": [" << std::endl;
    for (size_t i = 0; i < results.size(); i++)
    {
        output << "\t\t{" << std::endl;
        LogSingleResult(results[i]);
        output << "\t\t}" << (i == results.size()-1? "" : ",") << std::endl;
    }
    output << "\t]\n}" << std::endl;
}