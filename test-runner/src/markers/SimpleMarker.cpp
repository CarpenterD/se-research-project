#include <iostream>
#include "SimpleMarker.hpp"

SimpleMarker::SimpleMarker(TestConfig config){
#ifdef DEBUG
    std::cout << "Creating SimpleMarker" << std::endl;
#endif
    SimpleOptions::Initialise(config.Options);
}

TestResult SimpleMarker::Mark(std::string file){
    TestResult result;
    result.testName = "Simple Marker";
    result.testDescription = "Simple Marker - marking is simple, right?";
    result.marksTotal = 5;
    result.marksAwarded = 3 + BonusMarks;
    result.remarks = FeedbackMessage;
    return result;
}
