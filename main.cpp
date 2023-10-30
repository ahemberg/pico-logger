#include <stdio.h>
#include <iostream>
#include <algorithm>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/rtc.h"
#include "hardware/gpio.h"
#include "secrets.hpp"
#include "sensor/dht22.hpp"
#include "sensor/system.hpp"

#include "network/timeutils.hpp"
#include "influx/influxclient.hpp"

char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASS;

// TODO DUplicate code! Lets build a clock-class
uint32_t get_unix_timestamp()
{
    return (uint32_t)get_time();
}

bool connect_wifi(char *ssid, char *pass)
{
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_UK))
    {
        printf("failed initialize\n");
        cyw43_arch_deinit();
        return false;
    }
    printf("initialized\n");

    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms(ssid, pass, CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        printf("failed to connect\n");
        cyw43_arch_deinit();
        return false;
    }
    printf("connected\n");
    return true;
}

void disconnect_wifi()
{
    void cyw43_arch_disable_sta_mode();
    cyw43_arch_deinit();
}

void block_until_wifi_connected(char *ssid, char *pass, uint32_t timeout)
{
    uint32_t delay_backoff = 1000;
    uint32_t time_waited = 0;
    while (!connect_wifi(ssid, pass))
    {
        std::cout << "Failed toconnect to Wifi. Will retry in " << delay_backoff / 1000 << " seconds" << std::endl;
        sleep_ms(delay_backoff); // todo: should be a deep sleep after a couple of retries.
        if (timeout != 0 && time_waited >= timeout)
        {
            return;
        }
        delay_backoff += 10000;
    }
}

// TODO will block forever if wifi not connected when called.
void block_until_rtc_updated(uint32_t timeout)
{
    uint32_t delay_backoff = 1;
    uint32_t time_waited = 0;
    while (!update_rtc_from_ntp())
    {
        std::cout << "Failed to update RTC. Will retry in " << delay_backoff / 1000 << " seconds" << std::endl;
        sleep_ms(delay_backoff); // Todo: this must be a deep sleep after a couple of tries
        if (timeout != 0 && time_waited >= timeout)
        {
            return;
        }
        delay_backoff += 10000;
    }
}
uint32_t total_ticks = 0;

int main()
{
    std::list<Sensor *> sensors = {new Dht22("pico_dev", 15), new System("pico_dev")};

    stdio_init_all();

    // Connect WIFI
    block_until_wifi_connected(ssid, pass, 0);

    // Update rtc with current time
    rtc_init();
    block_until_rtc_updated(0);

    // Disconnect wifi
    disconnect_wifi();

    int ticks = 0;

    while (true)
    {
        std::cout << "Ticks: " << ticks << " Total ticks: " << total_ticks << " Mem: " << System::getFreeHeap() << std::endl;

        std::for_each(sensors.begin(), sensors.end(), [&](Sensor *s)
                      { s->measure(); });

        ticks++;
        total_ticks++;

        if (ticks == 30)
        {
            // Connect WIFI
            block_until_wifi_connected(ssid, pass, 60000);

            std::for_each(sensors.begin(), sensors.end(), [&](Sensor *s)
                          {
                uint8_t failed_attempts = 0;
                while(s->has_measurements() && failed_attempts < 5) {
                // s.measure() add this call to get datapoints during transfer. 
    
                if (s->post_measurements(20, INFLUX_SERVER, INFLUX_PORT, INFLUX_DATABASE, INFLUX_USER, INFLUX_PASSWORD))
                {
                    std::cout << "Sending data success!" << std::endl;
                } else
                {
                    // TODO Two failures in a row crashes the program. But only if dns resolves.
                    std::cout << "Failed to send data!" << std::endl;
                    failed_attempts++;
                }
            } });
        }
        sleep_ms(1000);
    }
}
