#pragma once

#include <stdexcept>
#include <memory>
#include <vector>

#include "pin.h"


class Component {
public: 

    Pin& addPin() {

        pins_.push_back(std::unique_ptr<Pin>(new Pin()));

        return *pins_.back();
    }

    Pin& findPin(int id) {
        for (const auto& p: pins_) {
            if (p->getId() == id) {
                return *p;
            }
        }

        throw std::invalid_argument("Id not found");
    }
    
private:
    std::vector<std::unique_ptr<Pin>> pins_;
};
