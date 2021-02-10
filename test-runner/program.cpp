#include <iostream>
#include <string>
#include <array>
#include <vector>

#include "markers/MarkerRegistry.hpp"
#include "writers/WriterRegistry.hpp"

int main(int argc, char *argv[]){

    //load xml configurations here etc.

    ResultWriter* writer = WriterRegistry::CreateWriter("DebugLogger", "<config xml=\"no xml\"/>");
    
    std::array<std::string, 2> markers = {"StubMarker", "SimpleMarker"};
    std::vector<TestResult> results (markers.size());
    for (size_t i = 0; i < markers.size(); i++)
    {
        Marker* someMarker = MarkerRegistry::CreateMarker(markers[i], "");
        std::cout << "HERE" << std::endl;
        TestResult result = someMarker->Mark("helloworld.c");
        std::cout << "THERE" << std::endl;
        results.push_back(result);
        std::cout << "WHERE" << std::endl;
    }
    writer->OutputResults(results);
    
    return EXIT_SUCCESS;
}