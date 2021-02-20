#ifndef ARG_PARSER
#define ARG_PARSER

#include <string>
#include <vector>

struct Arguments
{
    std::vector<std::string> filePaths;
    std::string configPath;
    std::string outputPath;
};

class ArgumentParser
{
    public:
        Arguments ParseArgs(int argc, char* argv[]);
    private:
        void PrintUsage();
};

#endif