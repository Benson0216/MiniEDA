#pragma once

#include <stdexcept>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <cstddef>


inline std::vector<std::string> split(
    const std::string& line
) 
{
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;

    while (iss >> token)
    {
        tokens.push_back(token);
    }

    return tokens;
}


inline std::pair<std::string, std::string> splitPinRef(
    const std::string& ref
) 
{
    std::size_t pos = ref.rfind(".");
    
    if (pos == std::string::npos) {
        throw std::invalid_argument(
            "Invalid reference: " + ref);
    }

    std::string comp_name = ref.substr(0, pos);
    std::string pin_number = ref.substr(pos + 1);

    if (comp_name.empty() || 
        pin_number.empty()) {
        throw std::invalid_argument(
            "Invalid reference: " + ref);
    }

    return {comp_name, pin_number};
}


inline std::pair<std::string, std::string> splitPinDef(
    const std::string& def
)
{
    std::size_t pos = def.find('=');

    if (pos == std::string::npos ) {
        throw std::invalid_argument(
            "Invalid definition: " + def);
    }

    std::string number = def.substr(0, pos);
    std::string name = def.substr(pos + 1);

    if (number.empty() || 
        name.empty()) {
        throw std::invalid_argument(
            "Invalid definition: " + def);
    }

    return {number, name};
}
