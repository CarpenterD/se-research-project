#ifndef CONFIGURATIONS
#define CONFIGURATIONS

#include <string>
#include <vector>

struct ConfigOption
{
    std::string Name;
    std::string Value;
};

struct TestConfig
{
    std::string TestName;
    /*
      std test options go here
    */
    std::vector<ConfigOption> Options; 
};

struct WriterConfig
{
    std::string WriterName;
    std::vector<ConfigOption> Options; 
};

struct RootConfig
{
    // Other standard configurations go here
    std::vector<TestConfig> Tests;
    WriterConfig Writer;
};

#endif