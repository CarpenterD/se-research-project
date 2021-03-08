#ifndef CONFIG_OPTION
#define CONFIG_OPTION

#include <string>
#include <vector>

struct ConfigOption
{
    std::string Name;
    std::string Value;
    std::vector<std::string> Values;
    bool IsListProperty;
};

#endif