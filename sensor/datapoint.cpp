#include "datapoint.hpp"

std::string Datapoint::to_line() const {

    std::ostringstream os;
    os << entity << "=" << value;

    return os.str();
}