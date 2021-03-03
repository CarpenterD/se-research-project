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
#include "utils/ConfigReader.hpp"

int main(int argc, char *argv[]){
    ArgumentParser argParser;
    Arguments args = argParser.ParseArgs(argc, argv);
    
    RootConfig config;

    ResultWriter* writer;
    try
    {
        //load xml configurations here etc.
        config = ConfigReader::ReadConfigurations(args.configPath);

        // initialise output
        std::ostream* outStream = CreateOutputStream(args.outputPath);
        writer = WriterRegistry::CreateWriter(outStream, config.Ouput);
    }
    catch(const std::exception& e)
    {
        std::cerr << "error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    // mark
    std::vector<TestResult> results;
    for (std::size_t i = 0; i < config.Tests.size(); i++)
    {
        try
        {
            Marker* someMarker = MarkerRegistry::CreateMarker(config.Tests[i]);
            results.push_back( someMarker->Mark(args.filePaths[0]) );
        }
        catch(const std::exception& e)
        {
            std::cerr << "error: In test '" + config.Tests[i].TestName + "': " << e.what() << std::endl;
            // could exit here - depends on test config
        }
    }

    writer->OutputResults(results);

    return EXIT_SUCCESS;
}