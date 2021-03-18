#include "ConfigConverter.hpp"

#include <iostream>

template <>
std::string ConfigConverter::ConvertOption<std::string>(ConfigOption *option){
    if (option == NULL)
    {
        throw std::invalid_argument("Option not found.");
    }
    return option->Value;
}

template <>
int ConfigConverter::ConvertOption<int>(ConfigOption *option){
    if (option == NULL)
    {
        throw std::invalid_argument("Option not found.");
    }
    return std::stoi(option->Value);
}
