#ifndef CONFIG_CONVERTER
#define CONFIG_CONVERTER

#include <vector>
#include <stdexcept>
#include "utils/configuration/ConfigOption.hpp"

namespace ConfigConverter
{
    // int ConvertOption(ConfigOption &option);
    // int ConvertOption(ConfigOption &option, int defaultValue);
    // std::vector<int> ConvertOption(ConfigOption &option);


    // Anon. namespace for template internals ---
    namespace
    {

    }

    // base template and variations -------------
    template <class T>
    T ConvertOption(ConfigOption *option)
    {
        throw std::invalid_argument("Not Implemented. Unable to convert the selected type.");
    }

    template <class T>
    T ConvertOption(ConfigOption *option, T defaultValue)
    {
        try
        {
            return ConvertOption<T>(option);
        }
        catch(const std::exception& e)
        {
            return defaultValue;
        }
    }

    template <class T>
    void ConvertOption(ConfigOption *option, std::vector<T> &collection)
    {
        if (option == NULL)
        {
            throw std::invalid_argument("Option not found.");
        }
        for (size_t i = 0; i < option->Values.size(); i++)
        {
            try
            {
                collection.push_back(ConvertOption<T>(option->Values[1]));
            }
            catch (const std::exception& e)
            {
                // bad conversion -> do nothing
            }
        }
    }

    // template specialisations -----------------
    template <>
    std::string ConvertOption<std::string>(ConfigOption *option);

    template <>
    int ConvertOption<int>(ConfigOption *option);
};

#endif