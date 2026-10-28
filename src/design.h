#pragma once

#include <stdexcept>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstddef>
#include <utility>

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
        //Push back item into vector before register pointer in map
        //Avoid pointer referenced at released object
        components_.push_back(std::move(comp));
        Component& ref = *components_.back();
        component_map_[ref.getName()] = &ref;

        return ref;
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
        nets_.push_back(std::move(net));
        Net& ref = *nets_.back();
        net_map_[ref.getName()] = &ref;

        return ref;
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

    const Component& findComponent(
        const std::string& name
    ) const 
    {
        auto it = component_map_.find(name);
        if (it == component_map_.end()) {
            throw std::invalid_argument(
                "component not found: " + name);
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

    const Net& findNet(
        const std::string& name
    ) const
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

    std::vector<const Component*> components() const 
    {
        std::vector<const Component*> result;
        result.reserve(components_.size());
        for (const auto& c : components_) {
            result.push_back(c.get());
        }
        return result;
    }

    std::vector<const Net*> nets() const 
    {
        std::vector<const Net*> result;
        result.reserve(nets_.size());
        for (const auto& n : nets_) {
            result.push_back(n.get());
        }
        return result;
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