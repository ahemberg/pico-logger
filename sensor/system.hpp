#ifndef SYSTEM_HPP 
#define SYSTEM_HPP

#include <stdio.h>
#include <list>
#include <math.h>
#include "pico/stdlib.h"
#include "sensor.hpp"
#include "measurement.hpp"
#include "datapoint.hpp"
#include <time.h>
#include "network/timeutils.hpp"
#include <malloc.h>

#include "pico/cyw43_arch.h"

class System: public Sensor {
    public:
        System(std::string _host) : Sensor("system_metrics", _host) { };
        virtual void measure();
        static uint32_t getFreeHeap();
    private:
        static uint32_t getTotalHeap();
};

#endif