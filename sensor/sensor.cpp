#include "sensor.hpp"

bool Sensor::post_measurements(uint8_t n, std::string server_address, uint16_t port, std::string database, std::string username, std::string password) {

    uint8_t n_to_send = n < measurements.size() ? n : measurements.size();
    std::cout << measurements.size()<< std::endl;
    std::list<Measurement> measurements_to_send = popn(n_to_send);
    std::cout << measurements.size() << std::endl;

    if (!post_to_influx(server_address, port, database, username, password, to_payload(measurements_to_send))) {
    //if (false) {
        //TODO: figure out which messages that failed
        push_all(measurements_to_send);
        return false;
    }
    return true;
}

bool Sensor::has_measurements() {
    return !measurements.empty();
}

std::string Sensor::to_payload(std::list<Measurement> &_measurements) const
{
    std::ostringstream os;
    for (const auto &measurement : _measurements)
    {
        os << name << ","
           << "host=" << host << " " << measurement.to_line() << "\n";
    }
    return os.str();
}

std::string Sensor::to_payload() const
{
    std::ostringstream os;
    for (const auto &measurement : measurements)
    {
        os << name << ","
           << "host=" << host << " " << measurement.to_line() << "\n";
    }
    return os.str();
}

std::list<Measurement> Sensor::popn(uint16_t n)
{
    std::list<Measurement> _measurements;
    uint16_t n_to_pop = n < measurements.size() ? n : measurements.size();

    for (int i = 0; i < n_to_pop; i++)
    {
        _measurements.push_back(pop());
    }
    return _measurements;
}

void Sensor::push_all(std::list<Measurement> &_measurements)
{
    for (const auto &measurement : _measurements)
    {
        push(measurement);
    }
}

/**
 * Adds a measurement. If the number of stored measurements after adding the new measurement
 * is larger than MAX_MEASUREMENTS then the oldest measurement will be deleted.
 *
 * TODO: A compacting feature (dropping every other measurement, averaging etc)
 */
void Sensor::push(Measurement measurement)
{
    measurements.push_front(measurement);
    if (measurements.size() > MAX_MEASUREMENTS)
    {
        measurements.pop_back();
    }
}
/**
 * Returns the oldest measurement. Removes the measurement from the internal queue
 */
Measurement Sensor::pop()
{
    Measurement m = measurements.back();
    measurements.pop_back();
    return m;
}

void Sensor::clear()
{
    measurements.clear();
}