#pragma once

#include <functional>
#include <vector>

#include "pin.h"


class Net {
public: 
    void append(const Pin& pin) {
        pins_.push_back(pin);
    }

    const std::vector<std::reference_wrapper<const Pin>>& pins() const { 
        return pins_; 
    }

private:
    std::vector<std::reference_wrapper<const Pin>> pins_;
};