# pragma once

#include <string>
#include "measurement.hpp"

class Sensor {
    public:
        Sensor(std::string _name, std::string _host) : name(_name), host(_host), measurements() {};
        std::string to_payload();
        virtual void measure() = 0;
    private:
        const std::string name;
        const std::string host;
        std::list<Measurement> measurements;
};