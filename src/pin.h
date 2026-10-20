#pragma once


class Pin{
public: 
    void setId(int value){
        id_ = value;
    }

    int getId() const {
        return id_;
    }

    Pin(const Pin&) = delete;
    
    Pin& operator=(const Pin&) = delete;

private:
    friend class Component;
    Pin(): id_(0) {}
    int id_;
};
