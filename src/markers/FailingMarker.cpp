#include <iostream>
#include <exception>

#include "FailingMarker.hpp"

FailingMarker::FailingMarker(TestConfig config){
#ifdef DEBUG
    std::cout << "Creating FailingMarker" << std::endl;
#endif
}

TestResult FailingMarker::Mark(std::string file){
    TestResult result;
    result.testName = "Failing Marker";
    result.testDescription = "Designed to always fail and test error handling";
    result.marksTotal = 0;
    result.marksAwarded = 0;
    result.remarks = "If you are seeing this, the marker did not fail.";

    throw std::domain_error("Intended failure. Checking error handling.");

    return result;
}
