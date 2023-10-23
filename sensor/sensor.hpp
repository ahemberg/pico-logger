#ifndef SENSOR_HPP 
#define SENSOR_HPP

#define MAX_MEASUREMENTS 600

#include <string>
#include <iostream>
#include <sstream>
#include <deque>

#include "measurement.hpp"
#include "influx/influxclient.hpp"

class Sensor {
    public:
        Sensor(std::string _name, std::string _host) : name(_name), host(_host), measurements() {};
        std::string to_payload(std::list<Measurement> &_measurements) const;
        std::string to_payload() const; //TODO remove
        virtual void measure() = 0;
        void clear();
        bool post_measurements(uint8_t n, std::string server_address, uint16_t port, std::string database, std::string username, std::string password);
        bool has_measurements();
    protected:
        const std::string name;
        const std::string host;
        void push_all(std::list<Measurement> &_measurements); //Whatever is pushed on will remove what is currently here. This should be used with caution 
        void push(Measurement measurement);
        Measurement pop();
        std::list<Measurement> popn(uint16_t n);
        std::deque<Measurement> measurements; //OOM happens at roughly 500 elements. TODO: Bound this, use a ring-buffer so that we can write forever.
};

#endif