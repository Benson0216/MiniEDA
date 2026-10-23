#pragma once

#include <stdexcept>
#include <sstream>
#include <string>
#include <utility>
#include <vector>


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
    size_t pos = ref.rfind(".");
    std::string comp_name = ref.substr(0, pos);
    std::string pin_number = ref.substr(pos + 1);

    if (pos == std::string::npos ||
        comp_name.empty() || 
        pin_number.empty()) {
        throw std::invalid_argument(
            "Invalid reference: " + ref);
    }

    return {comp_name, pin_number};
}
