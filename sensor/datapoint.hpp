#ifndef DATAPOINT_HPP 
#define DATAPOINT_HPP

#include <string>
#include <iostream>
#include <sstream>

class Datapoint {
    public:
        Datapoint(std::string _entity, float _value) : entity(_entity), value(_value) {};
        std::string to_line() const;
    private:
        const std::string entity;
        const float value;
};

#endif