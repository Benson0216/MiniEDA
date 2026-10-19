#pragma once

#include <stdexcept>
#include <memory>
#include <vector>

#include "pin.h"


class Component {
public: 

    Pin& addPin() {

        auto pin = std::make_unique<Pin>();

        pins_.push_back(std::move(pin));

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
