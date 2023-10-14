#include "dht22.hpp"

void Dht22::measure() {
    measurements.push_back(read_from_dht(pin, MAX_TIMINGS));
}

Measurement Dht22::read_from_dht(const uint dht_pin, const uint max_timings) {
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
        return Measurement(get_unix_timestamp(), datapoints);

    } else {
        // TODO in this case we have bad data. Should raise an exception!
        std::cout << "Failed to measure" << std::endl;
        std::cout << "Got: data{" << data[0] << "," << data[1] << "," << data[2]<< "," << data[3]<< "," << data[4] << "}" << std::endl;
        return Measurement(0, {});
    }
}

uint32_t Dht22::get_unix_timestamp() {
    // TODO: assumes that the rtc is correclty set. In the time-utils class this should be verifiable
    datetime_t t;
    //Check RTC initialised
    rtc_get_datetime(&t);

    //Make a time_t from year month day and so on
    //Convert this to unix timestamp using standard c. Don't reinvent the wheel

    struct tm tm_time;

    tm_time.tm_year = t.year - 1900;
    tm_time.tm_mon = t.month -1;
    tm_time.tm_mday = t.day;
    tm_time.tm_hour = t.hour;
    tm_time.tm_min = t.min;
    tm_time.tm_sec = t.sec;
    tm_time.tm_isdst = -1; //TODO this might be a problem. But its always UTC so maybe not?

    
    return (uint32_t)mktime(&tm_time);
}