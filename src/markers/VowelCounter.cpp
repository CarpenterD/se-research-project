#include "VowelCounter.hpp"

#include <iostream>
#include <fstream>
#include "../utils/FileIO.hpp"

VowelCounter::VowelCounter(TestConfig config){
#ifdef DEBUG
    std::cout << "Creating VowelCounter" << std::endl;
#endif
}

TestResult VowelCounter::Mark(std::string file){
    TestResult result;
    result.testName = "VowelCounter";
    result.testDescription = "Calculated the average number of vowels per line";
    result.testInfo = "Case insentive. Carriage return characters are ignored.";
    result.marksTotal = 2;

    // process file
    int vowels = 0;
    int lines = 1;
    int vowelsThisline = 0;
    char c;

    std::istream* stream = CreateInputStream(file);
    
    while (stream->get(c)){
        if (c=='a' || c=='e' || c=='i' || c=='o' || c=='u'|| c=='A' || c=='E' || c=='I' || c=='O' || c=='U') {
            vowels++;
            vowelsThisline++;
        } else if (c=='\n') {
            if (vowelsThisline == 0){
                result.feedback.push_back(std::pair<SourceLocation, std::string>(SourceLocation(file, lines, 0), "No vowels found on this line."));
            }
            lines++;
            vowelsThisline = 0;
        }
    }

    double ratio = vowels / (double) lines;
    if (ratio > 2){
        result.marksAwarded = 2;
        result.remarks = "Well done. " + std::to_string(ratio) + "v/l is a great ratio!";
    } else if (ratio > 1){
        result.marksAwarded = 2;
        result.remarks = "Not bad. " + std::to_string(ratio) + "v/l is good, but there is room for improvement.";
    } else {
        result.marksAwarded = 0;
        result.remarks = std::to_string(ratio) + "v/l is too low, try to avoid too many abbreviations.";
    }

    return result;
}
