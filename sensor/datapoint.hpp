#pragma once

#include <string>

class Datapoint {
    public:
        Datapoint(std::string _entity, float _value) : entity(_entity), value(_value) {};
        // virtual ~Datapoint(); TODO: Do I need this?
        std::string to_line();

    private:
        const std::string entity;
        const float value;
};