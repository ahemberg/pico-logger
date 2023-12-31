#include "dht22.hpp"

void Dht22::measure() {
    push(read_from_dht(pin));
}

Measurement Dht22::read_from_dht(const uint dht_pin) {
    const uint max_timings = 85;
    int data[5] = {0, 0, 0, 0, 0};
    uint last = 1;
    uint j = 0;

    gpio_set_dir(dht_pin, GPIO_OUT);
    gpio_put(dht_pin, 0);
    sleep_ms(20);
    gpio_set_dir(dht_pin, GPIO_IN);

    //gpio_put(PICO_DEFAULT_LED_PIN, 1);
    
    for (uint i = 0; i < max_timings; i++) {
        uint count = 0;
        while (gpio_get(dht_pin) == last) {
            count++;
            sleep_us(1);
            if (count == 255) break;
        }
        last = gpio_get(dht_pin);
        if (count == 255) break;

        if ((i >= 4) && (i % 2 == 0)) {
            data[j / 8] <<= 1;
            if (count > 46) data[j / 8] |= 1;
            j++;
        }
    }
    //gpio_put(PICO_DEFAULT_LED_PIN, 0);

    if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
        float humidity = (float) ((data[0] << 8) + data[1]) / 10;
        if (humidity > 100) {
            humidity = data[0];
        }
        float temp_celsius = (float) (((data[2] & 0x7F) << 8) + data[3]) / 10;
        if (temp_celsius > 125) {
            temp_celsius = data[2];
        }
        if (data[2] & 0x80) {
            temp_celsius = -temp_celsius;
        }

        std::list<Datapoint> datapoints = {Datapoint("humidity", humidity), Datapoint("temperature", temp_celsius)};
        return Measurement((uint32_t)get_time(), datapoints);

    } else {
        // TODO in this case we have bad data. Should raise an exception!
        std::cout << "Failed to measure" << std::endl;
        std::cout << "Got: data{" << data[0] << "," << data[1] << "," << data[2]<< "," << data[3]<< "," << data[4] << "}" << std::endl;
        return Measurement(0, {});
    }
}