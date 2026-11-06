#pragma once

#include <unordered_set>
#include <vector>

#include "design.h"


struct PinRef {
    const Component* component;
    const Pin* pin;
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