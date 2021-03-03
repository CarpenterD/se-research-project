#include <iostream>
#include "SimpleMarker.hpp"
#include "utils/Configurations.hpp"

SimpleMarker::SimpleMarker(TestConfig config){
    // std::cout << "Creating SimpleMarker (config=" << config << ")" << std::endl;
}

TestResult SimpleMarker::Mark(std::string file){
    TestResult result;
    result.testName = "Simple Marker";
    result.testDescription = "Simple Marker - marking is simple, right?";
    result.marksTotal = 5;
    result.marksAwarded = 3;
    result.remarks = "For a brick, he flew pretty good!";
    return result;
}
