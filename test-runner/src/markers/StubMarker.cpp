#include <iostream>
#include "StubMarker.hpp"
#include "utils/Configurations.hpp"

StubMarker::StubMarker(TestConfig config){
    // std::cout << "Creating StubMarker (config=" << config << ")" << std::endl;
}

TestResult StubMarker::Mark(std::string file){
    TestResult result;
    result.testName = "StubMarker";
    result.testDescription = "StubMarker - don't take results from this one too seriously...";
    result.remarks = "This just a stub, but this code probably looks alright.";
    result.marksTotal = 5;
    result.marksAwarded = 5;
    return result;
}
