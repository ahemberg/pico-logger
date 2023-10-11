#ifndef SENSOR_HPP 
#define SENSOR_HPP

#include <string>
#include <iostream>
#include <sstream>

#include "measurement.hpp"

class Sensor {
    public:
        Sensor(std::string _name, std::string _host) : name(_name), host(_host), measurements() {};
        std::string to_payload() const;
        virtual void measure() = 0;
    protected:
        const std::string name;
        const std::string host;
        std::list<Measurement> measurements;
};

#endif