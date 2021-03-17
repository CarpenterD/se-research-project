#include <iostream>
#include "StubMarker.hpp"

StubMarker::StubMarker(TestConfig config){
#ifdef DEBUG
    std::cout << "Creating StubMarker" << std::endl;
#endif
    StdOptions::Initialise(config.Options);
}

TestResult StubMarker::Mark(std::string file){
    TestResult result;
    result.testName = "StubMarker";
    result.testDescription = "StubMarker - don't take results from this one too seriously...";
    result.remarks = "This just a stub, but this code probably looks alright.";
    result.marksTotal = MarksAvailable;
    result.marksAwarded = MarksAvailable;
    return result;
}
