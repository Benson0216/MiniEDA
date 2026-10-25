#pragma once

#include <stdexcept>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

#include "pin.h"


class Component {
public: 
    explicit Component(std::string name)
        : name_(std::move(name)) {}

    const std::string& getName() const { return name_; }

    Pin& addPin(
        std::string number,
        std::string name
    ) 
    {
        const bool duplicated = std::any_of(
            pins_.begin(), pins_.end(),
            [&](const std::unique_ptr<Pin>& p) {
                return p->getNumber() == number;
            });
        
        if (duplicated) {
            throw std::invalid_argument(
                "duplicate pin number: " + name_ + "." + number
            );
        }

        pins_.push_back(std::unique_ptr<Pin>(
                new Pin(std::move(number), std::move(name))
            )
        );

        return *pins_.back();
    }

    Pin& findPin(
        const std::string& number
    ) 
    {
        for (const auto& p: pins_) {
            if (p->getNumber() == number) {
                return *p;
            }
        }

        throw std::invalid_argument(
            "pin not found: " + name_ + "." + number
        );
    }
    
private:
    std::string name_;
    std::vector<std::unique_ptr<Pin>> pins_;
};
