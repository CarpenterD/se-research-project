#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>

#include "testing/TestResult.hpp"
#include "markers/MarkerRegistry.hpp"
#include "writers/WriterRegistry.hpp"
#include "utils/ArgumentParser.hpp"
#include "utils/FileIO.hpp"

int main(int argc, char *argv[]){
    ArgumentParser argParser;
    Arguments args = argParser.ParseArgs(argc, argv);
    
    ResultWriter* writer;
    try
    {
        //load xml configurations here etc.

        // initialise output
        std::ostream* outStream = CreateOutputStream(args.outputPath);
        writer = WriterRegistry::CreateWriter("DebugLogger", outStream, "<config xml=\"no xml\"/>");
    }
    catch(const std::exception& e)
    {
        std::cerr << "error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    // mark
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