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

template <>
bool ConfigConverter::ConvertTo<bool>(std::string str){
    if (str.size() == 1) {
        return str[0] != '0';
    }
    if (str == "true") {
        return true;
    }
    if (str == "false") {
        return false;
    }
    throw std::invalid_argument("Invalid string input for bool type: \""+str+"\". Valid values: [\"0\", \"1\", \"true\", \"false\"]");
}
