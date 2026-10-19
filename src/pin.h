#pragma once


class Pin{
public: 
    Pin(): id_(0) {}

    void setId(int value){
        id_ = value;
    }

    int getId() const {
        return id_;
    }

private:
    int id_;
};
