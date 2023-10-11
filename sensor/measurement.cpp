#include "measurement.hpp"

std::string Measurement::to_line() const {
    std::ostringstream os;

    for (const auto& datapoint: datapoints) {
        os << datapoint.to_line() << ",";
    }
    os.seekp(-1, os.cur);

    os << " " << timestamp;

    return os.str();
}