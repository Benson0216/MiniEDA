#pragma once

#include <stdexcept>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include "component.h"
#include "net.h"


class Design {
public:
    Component& addComponent(
        std::string name
    )
    {
        if (component_map_.find(name) != component_map_.end()) 
        {
            throw std::invalid_argument(
                "duplicate component name: " + name
            );
        }

        auto comp = std::make_unique<Component>(std::move(name));
        component_map_[comp->getName()] = comp.get();
        components_.push_back(std::move(comp));

        return *components_.back();
    }

    Net& addNet(
        std::string name
    )
    {
        if (net_map_.find(name) != net_map_.end()) 
        {
            throw std::invalid_argument(
                "duplicate net name: " + name
            );
        }

        auto net = std::make_unique<Net>(std::move(name));
        net_map_[net->getName()] = net.get();
        nets_.push_back(std::move(net));

        return *nets_.back();
    }

    Component& findComponent(
        const std::string& name
    ) 
    {
        auto it = component_map_.find(name);
        if (it == component_map_.end()) {
            throw std::invalid_argument(
                "component not found: " + name
            );
        }

        return *it->second;
    }

    Net& findNet(
        const std::string& name
    )
    {
        auto it = net_map_.find(name);
        if (it == net_map_.end()) {
            throw std::invalid_argument(
                "net not found: " + name
            );
        }

        return *it->second;
    }

    std::size_t componentCount() const
    {
        return components_.size();
    }

    std::size_t netCount() const
    {
        return nets_.size();
    }

private:
    //Delete last
    std::vector<std::unique_ptr<Component>> components_; 
    //Delete first (due to lifetime is shorter than component)
    std::vector<std::unique_ptr<Net>> nets_;
    //Reference is not owning pointer, so delete first
    std::unordered_map<std::string, Component*> component_map_;
    std::unordered_map<std::string, Net*> net_map_;
};