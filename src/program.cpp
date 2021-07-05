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



/* DECLATATIONS */
// Handles errors thrown during the marking process
bool HandleMarkingException(const std::exception &e, TestConfig &cfg, std::vector<TestResult> &results);
// logs test config information
void LogTestConfig(TestConfig &cfg);



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

    bool stopTesting = false;
    
    // mark
    std::vector<TestResult> results;
    for (std::size_t i = 0; i < config.Tests.size(); i++)
    {
        TestConfig &cfg = config.Tests[i];
        try
        {
            #ifdef DEBUG
                LogTestConfig(cfg);
            #endif

            Marker* someMarker = MarkerRegistry::CreateMarker(cfg);
            results.push_back( someMarker->Mark(args.filePaths[0]) );
        }
        catch(const std::exception& e)
        {
            stopTesting = HandleMarkingException(e, cfg, results);
            if (stopTesting) {
                break;
            }
        }
    }

    writer->OutputResults(results);

    if (stopTesting) {
        std::cerr << "Note: Marking has been stopped by the result of the last marker. Some checks may not have been run." << std::endl;
    }

    return EXIT_SUCCESS;
}



/* ERROR HANDLING */
bool HandleMarkingException(const std::exception &e, TestConfig &cfg, std::vector<TestResult> &results)
{
    std::string errMsg(e.what());

    #ifdef DEBUG
        std::clog << "error: '" + cfg.TestName + "' threw an exception -> \"" << errMsg << "\"" << std::endl;
    #endif

    TestResult failResult;
    failResult.testName = cfg.TestName + " (ERROR)";
    failResult.testDescription = "An error occurred during processing";
    failResult.testInfo = "Exception thrown -> \"" + std::string(errMsg) + "\"";
    failResult.marksAwarded = 0;
    failResult.marksTotal = 0;
    if (cfg.BreakOnFail) {
        failResult.remarks = "This marker is configured with \"BreakOnFail\"=true. Further markers will not be run.";
    }

    results.push_back(failResult);

    return cfg.BreakOnFail;
}

/* LOGGING */
void LogTestConfig(TestConfig &cfg)
{
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
}