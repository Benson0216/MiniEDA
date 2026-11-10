#pragma once

#include <unordered_set>
#include <vector>
#include <unordered_map>

#include "design.h"


struct PinRef {
    const Component* component;
    const Pin* pin;
};


struct PinConflict {
    PinRef pin;
    std::vector<const Net*> nets;   // 該 Pin 所屬的所有不同 Net，至少 2 個
};


inline std::vector<PinRef> findFloatingPins(
    const Design& design
)
{
    std::unordered_set<const Pin*> connected_pins;

    for (const Net* net: design.nets())
    {
        for (const Pin& pin: net->pins())
        {
            connected_pins.insert(&pin);
        }
    }

    std::vector<PinRef> pin_refs;

    for (const Component* comp: design.components())
    {
        for (const Pin* pin: comp->pins())
        {
            if (connected_pins.find(pin) == 
                connected_pins.end())
                {
                    pin_refs.push_back({comp, pin});
                }
        }
    }

    return pin_refs;
}


inline std::vector<const Net*> findDanglingNets(
    const Design& design
)
{
    std::vector<const Net*> nets;

    for (const Net* net: design.nets())
    {
        std::unordered_set<const Pin*> unique_pins;

        for (const Pin& pin: net->pins())
        {
            unique_pins.insert(&pin);
        }

        if (unique_pins.size() < 2)
        {
            nets.push_back(net);
        }
    }

    return nets;
}


inline std::vector<PinConflict> findPinsOnMultipleNets(
    const Design& design
)
{
    std::unordered_map<const Pin*, std::vector<const Net*>> nets_of_pin;

    for (const Net* n: design.nets())
    {
        for (const Pin& p: n->pins())
        {
            auto& nets = nets_of_pin[&p];
            if (nets.empty() || nets.back() != n) {
                nets.push_back(n);
            }
        }
    }

    std::vector<PinConflict> conflicts;

    for (const Component* comp : design.components())
    {
        for (const Pin* pin : comp->pins())
        {
            auto it = nets_of_pin.find(pin);
            if (it != nets_of_pin.end() && it->second.size() >= 2)
            {
                conflicts.push_back({{comp, pin}, it->second});
            }
        }
    }

    return conflicts;
}
