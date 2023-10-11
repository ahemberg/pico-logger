#ifndef MEASUREMENT_HPP 
#define MEASUREMENT_HPP

#include <list> 
#include <iostream>
#include <sstream>

#include "datapoint.hpp"

class Measurement {
    public:
        Measurement(int _timestamp, std::list<Datapoint> _datapoints) : timestamp(_timestamp), datapoints(_datapoints) {};
        std::string to_line() const;
    private:
        const uint32_t timestamp;
        const std::list<Datapoint> datapoints;
};

#endif