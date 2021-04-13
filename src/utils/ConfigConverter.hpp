#ifndef CONFIG_CONVERTER
#define CONFIG_CONVERTER

#include <vector>
#include <stdexcept>
#include "utils/configuration/ConfigOption.hpp"

namespace ConfigConverter
{
    // ConvertTo template by Mark Holmberg
    // https://gist.github.com/mark-d-holmberg/862733

    // Convert from a string to another type
    template <typename T>
    T ConvertTo(std::string str)
    {
        std::istringstream ss(str);
        T val;
        ss >> val;
        return val;
    }

    // base template and variations -------------
    template <class T>
    T ConvertOption(ConfigOption *option)
    {
        try
        {
            if (option == NULL) {
                throw std::invalid_argument("Option not found.");
            }
            return ConvertTo<T>(option->Value);
        }
        catch(const std::exception& e)
        {
            throw std::invalid_argument("Unable to convert the option -> " + std::string( e.what() ) );
        }
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
                collection.push_back(ConvertTo<T>(option->Values[i]));
            }
            catch (const std::exception& e)
            {
                // bad conversion -> do nothing
            }
        }
    }

    template <class T>
    void ConvertOption(ConfigOption *option, std::vector<T> &collection, const std::vector<T> &defaultCollection)
    {
        try
        {
            ConvertOption<T>(option, collection);
        }
        catch (const std::exception& e)
        {
            collection = defaultCollection;
        }
    }

    // template specialisations -----------------
    template <>
    std::string ConvertTo<std::string>(std::string str);

    template <>
    int ConvertTo<int>(std::string str);

    template <>
    bool ConvertTo<bool>(std::string str);
};

#endif