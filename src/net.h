#pragma once

#include <functional>
#include <vector>
#include <string>
#include <utility>

#include "pin.h"


class Net {
public:
    explicit Net(std::string name)
        : name_(std::move(name)) {}
    
    const std::string& getName() const { return name_; }

    void append(const Pin& pin) {
        pins_.push_back(pin);
    }

    const std::vector<std::reference_wrapper<const Pin>>& pins() const { 
        return pins_; 
    }

private:
    std::string name_;
    std::vector<std::reference_wrapper<const Pin>> pins_;
};