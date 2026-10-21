#pragma once

#include <string>
#include <utility>


class Pin{
public: 

    const std::string& getName() const { return name_; }

    const std::string& getNumber() const { return number_; }

    Pin(const Pin&) = delete;
    
    Pin& operator=(const Pin&) = delete;

private:
    friend class Component;
    Pin(std::string number, std::string name)
        : number_(std::move(number)), name_(std::move(name)) {}
    std::string number_;
    std::string name_;
};
