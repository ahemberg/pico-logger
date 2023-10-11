#include "sensor.hpp"

std::string Sensor::to_payload() const {

    std::ostringstream os;
    for (const auto& measurement: measurements) {
        os << name << "," << "host=" << host << " " << measurement.to_line() << "\n";
    }
    return os.str();
}