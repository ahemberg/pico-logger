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

#include "hardware/rtc.h"
#include <time.h>


class Dht22: public Sensor {
    public:
        Dht22(std::string _host, uint _pin) : Sensor("dht22", _host), pin(_pin) {
            gpio_init(pin);
        };
        virtual void measure();
    private:
        static Measurement read_from_dht(const uint dht_pin, const uint max_timings);
        const uint pin; //TODO: make this part of the constructor
        static const uint MAX_TIMINGS = 85; //This could just as well live inside the function
        static uint32_t get_unix_timestamp(); //TODO this should be in a helper class so move it!
};

#endif