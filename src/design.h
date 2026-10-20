#pragma once

#include <memory>
#include <vector>

#include "component.h"
#include "net.h"


class Design {
public:
    Component& addComponent() {
        
        auto comp = std::make_unique<Component>();
        
        components_.push_back(std::move(comp));

        return *components_.back();
    }

    Net& addNet() {

        auto net = std::make_unique<Net>();
        
        nets_.push_back(std::move(net));

        return *nets_.back();
    }

private:
    //Delete last
    std::vector<std::unique_ptr<Component>> components_; 
    //Delete first (due to lifetime is shorter than component)
    std::vector<std::unique_ptr<Net>> nets_;
};