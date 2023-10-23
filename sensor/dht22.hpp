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
#include "network/timeutils.hpp"
#include <time.h>


class Dht22: public Sensor {
    public:
        Dht22(std::string _host, uint _pin) : Sensor("dht22", _host), pin(_pin) {
            gpio_init(pin);
        };
        virtual void measure();
    private:
        static Measurement read_from_dht(const uint dht_pin);
        const uint pin; //TODO: make this part of the constructor
};

#endif