#include <iostream>
#include <string>
#include <array>
#include <vector>

#include "testing/TestResult.hpp"
#include "markers/MarkerRegistry.hpp"
#include "writers/WriterRegistry.hpp"
#include "utils/ArgumentParser.hpp"

int main(int argc, char *argv[]){
    ArgumentParser argParser;
    Arguments args = argParser.ParseArgs(argc, argv);
    
    //load xml configurations here etc.
    ResultWriter* writer = WriterRegistry::CreateWriter("DebugLogger", "<config xml=\"no xml\"/>");
    
    std::vector<std::string> markers = {"StubMarker", "SimpleMarker"};
    std::vector<TestResult> results;
    for (std::size_t i = 0; i < markers.size(); i++)
    {
        Marker* someMarker = MarkerRegistry::CreateMarker(markers[i], "");
        results.push_back( someMarker->Mark("helloworld.c") );
    }
    
    writer->OutputResults(results);
    
    return EXIT_SUCCESS;
}