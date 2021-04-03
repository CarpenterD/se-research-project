#include "ConfigConverter.hpp"

#include <iostream>

template <>
std::string ConfigConverter::ConvertTo<std::string>(std::string str){
    return str;
}

template <>
int ConfigConverter::ConvertTo<int>(std::string str){
    return std::stoi(str);
}
