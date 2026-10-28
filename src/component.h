#pragma once

#include <stdexcept>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <utility>

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

    const Pin& findPin (
        const std::string& number
    ) const
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

    std::vector<const Pin*> pins() const 
    {
        std::vector<const Pin*> result;
        result.reserve(pins_.size());
        for (const auto& p : pins_) {
            result.push_back(p.get());
        }
        return result;
    }
    
private:
    std::string name_;
    std::vector<std::unique_ptr<Pin>> pins_;
};
