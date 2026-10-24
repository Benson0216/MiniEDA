#pragma once

#include <stdexcept>
#include <sstream>
#include <string>

#include "string_utils.h"
#include "design.h"


inline void parseNetlist(
    std::istream& in, 
    Design& design
) 
{
    std::string line;
    int line_no = 0;
    while (std::getline(in, line)) {
        ++line_no;
        try {
            auto tokens = split(line);
            
            //Skip line if first token is empty or '#'
            if (tokens.empty() ||
                tokens[0][0] == '#')
            {
                continue;
            }

            if (tokens[0] == "COMP") 
            {
                if (tokens.size() < 2) {
                    throw std::invalid_argument(
                        "COMP requires a name");
                }

                auto& comp = design.addComponent(
                    tokens[1]
                );

                for (auto token = tokens.begin() + 2;
                     token != tokens.end();
                     ++token)
                {
                    auto pin_def_pair = splitPinDef(
                        *token
                    );
                    comp.addPin(
                        pin_def_pair.first,
                        pin_def_pair.second
                    );
                }
            }
            else if (tokens[0] == "NET")
            {
                if (tokens.size() < 2) {
                    throw std::invalid_argument(
                        "NET requires a name");
                }

                auto& net = design.addNet(
                    tokens[1]
                );
                
                for (auto token = tokens.begin() + 2;
                     token != tokens.end();
                     ++token)
                {
                    auto pin_ref_pair = splitPinRef(
                        *token
                    );
                    auto& comp = design.findComponent(
                        pin_ref_pair.first
                    );
                    auto& pin = comp.findPin(
                        pin_ref_pair.second
                    );
                    net.append(pin);
                }
            }
            else
            {
                throw std::invalid_argument(
                    "unknown keyword: " + tokens[0]
                );
            }
        } catch (const std::exception& e) {
            throw std::runtime_error(
                "line " + std::to_string(line_no) + ": " + e.what());
        }
    }
}
