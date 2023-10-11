#ifndef DHT22_HPP 
#define DHT22_HPP

#include <stdio.h>
#include <list>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "sensor.hpp"
#include "measurement.hpp"
#include "datapoint.hpp"

class Dht22: public Sensor {
    public:
        Dht22(std::string _host) : Sensor("dht22", _host) {};
        virtual void measure();
    private:
        static Measurement read_from_dht(const uint dht_pin, const uint max_timings);
        static const uint DHT_PIN = 15; //TODO: make this part of the constructor
        static const uint MAX_TIMINGS = 85; //This could just as well live inside the function

};

#endif