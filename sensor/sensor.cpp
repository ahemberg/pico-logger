#include "sensor.hpp"

std::string Sensor::to_payload() const {

    std::ostringstream os;
    for (const auto& measurement: measurements) {
        os << name << "," << "host=" << host << " " << measurement.to_line() << "d\n";
    }
    return os.str();
}

void Sensor::remove_all_measurements() {
    measurements.clear();
}