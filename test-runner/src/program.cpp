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
        TestConfig &cfg = config.Tests[i];
        try
        {
            #ifdef DEBUG
            //write config options for test
            std::clog << "ConfigOptions for '" + cfg.TestName + "':" << std::endl;
            for (size_t j = 0; j < cfg.Options.size(); j++)
            {
                std::clog << "\tName: '" << cfg.Options[j].Name;
                if (cfg.Options[j].IsListProperty){
                    std::clog << "' Values: ";
                    for (size_t k = 0; k < cfg.Options[j].Values.size()-1; k++)
                    {
                        std::clog << "'" << cfg.Options[j].Values[k] << "', ";
                    }
                    std::clog << "'" << cfg.Options[j].Values.back() << "'" << std::endl;
                }else{
                    std::clog << "' Value: '" << cfg.Options[j].Value << "'" << std::endl;
                }
            }
            #endif

            Marker* someMarker = MarkerRegistry::CreateMarker(cfg);
            results.push_back( someMarker->Mark(args.filePaths[0]) );
        }
        catch(const std::exception& e)
        {
            std::cerr << "error: In test '" + cfg.TestName + "': " << e.what() << std::endl;
            // could exit here - depends on test config
        }
    }

    writer->OutputResults(results);

    return EXIT_SUCCESS;
}