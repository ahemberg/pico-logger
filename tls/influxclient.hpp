#ifndef INFLUXCLIENT_HPP
#define INFLUXCLIENT_HPP

#include <stdio.h>
#include <iostream>
#include <sstream>

#include "sensor/measurement.hpp"
#include "tls_common.h"
#include "sensor/measurement.hpp"


//TODO, should handle failed requests
bool post_to_influx(std::string server_address, std::string database, std::string username, std::string password, std::string payload);

#endif