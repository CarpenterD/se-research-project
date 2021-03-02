#include "ArgumentParser.hpp"

#include <iostream>
#include <unistd.h>
#include <getopt.h>


const std::string ArgumentParser::defaultConfigFile = "../config/defaultconfig.xml";

void ArgumentParser::PrintUsage(){
    std::cout << "Usage:" << std::endl;
    std::cout << "\tprogramName [OPTIONS] <filesToMark>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t-h, --help:\n\t\tPrints usage information (this)" << std::endl;
    std::cout << "\t-c, --config [file]:\n\t\tPath to XML configuration file" << std::endl;
    std::cout << "\t-o, --outfile [file]:\n\t\tThe output file" << std::endl;
}

Arguments ArgumentParser::ParseArgs(int argc, char* argv[]){
    Arguments args;
    int optionsIndex = 0;
    int c;

    // get option arguments
    static option longOptions[] = {
        {"config", required_argument, 0, 'c'},
        {"help", no_argument, 0, 'h'},
        {"outfile", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    while (true){
        c = getopt_long (argc, argv, "c:ho:", longOptions, &optionsIndex);

        // found end of the options
        if (c == -1) {
            break;
        }

        switch (c) {
            case 'c':
                args.configPath = optarg;
                break;

            case 'o':
                args.outputPath = optarg;
                break;

            case 'h':
                PrintUsage();
                exit(EXIT_SUCCESS);
                break;

            case '?':
                PrintUsage();
                exit(EXIT_SUCCESS);
                break;

            default:
                abort();
                break;
        }
    }

    // get non-option arguments
    for (int pos=optind; pos < argc; pos++) {
        args.filePaths.push_back(argv[pos]);
    }

    if (args.filePaths.size() == 0) {
        std::cerr << "error: no input files" << std::endl;
        PrintUsage();
        exit(EXIT_FAILURE);
    }

    if (args.configPath == "") {
        args.configPath = defaultConfigFile;
    }

#ifdef DEBUG
    std::cout << "Input file(s):" << std::endl;
    for (size_t f = 0; f < args.filePaths.size(); f++)
    {
        std::cout << "\t" << args.filePaths[f] << std::endl;
    }

    std::cout << "Config file: " << args.configPath << std::endl;

    if (args.outputPath != "") {
        std::cout << "Output file: " << args.outputPath << std::endl;
    }
#endif

    return args;
}
