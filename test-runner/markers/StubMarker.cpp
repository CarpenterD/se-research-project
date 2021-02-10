#include <iostream>
#include "StubMarker.hpp"

StubMarker::StubMarker(std::string config){
    // std::cout << "Creating StubMarker (config=" << config << ")" << std::endl;
}

TestResult StubMarker::Mark(std::string file){
    TestResult result;
    std::cout << "stub 1" << std::endl;
    result.testName = "StubMarker";
    // result.testDescription = "StubMarker - don't take results from this one too seriously...";
    // result.remarks = "This just a stub, but this code probably looks alright.";
    result.marksTotal = 5;
    result.marksAwarded = 5;
    std::cout << "stub 2" << std::endl;
    return result;
}
