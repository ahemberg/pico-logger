
#pragma once

#include <list> 
#include "datapoint.hpp"

class Measurement {
    public:
        Measurement(int _timestamp, std::list<Datapoint> _datapoints) : timestamp(_timestamp), datapoints(_datapoints) {};
        std::string to_line();
    private:
        const int timestamp;
        const std::list<Datapoint> datapoints;
};