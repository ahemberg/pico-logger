#include <stdio.h>
#include <iostream>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/rtc.h"
#include "hardware/gpio.h"
#include "secrets.hpp"
#include "sensor/dht22.hpp"
#include "network/timeutils.hpp"
#include "tls/influxclient.hpp"


char ssid[] = SSID;
char pass[] = WPA_PASS;

//TODO DUplicate code! Lets build a clock-class
uint32_t get_unix_timestamp() {    
    return (uint32_t)get_time();
}

bool connect_wifi(char* ssid, char* pass) {
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

void disconnect_wifi() {
    void cyw43_arch_disable_sta_mode();
    cyw43_arch_deinit();
}

void block_until_wifi_connected(char* ssid, char* pass) {
    uint delay_backoff = 1000;
    while (!connect_wifi(ssid, pass)) {
        std::cout << "Failed toconnect to Wifi. Will retry in " << delay_backoff/1000 << " seconds" << std::endl;
        sleep_ms(delay_backoff);
        delay_backoff += 10000;
    }
}

// TODO will block forever if wifi not connected when called.
void block_until_rtc_updated() {
    uint delay_backoff = 1;
    while (!update_rtc_from_ntp()) {
        std::cout << "Failed to update RTC. Will retry in " << delay_backoff/1000 << " seconds" << std::endl;
        sleep_ms(delay_backoff);
        delay_backoff += 10000;
    }
}


int main()
{
    Dht22 dht_sensor = Dht22("pico_dev", 15);

    stdio_init_all();

    //Connect WIFI
    block_until_wifi_connected(ssid, pass);
    
    //Update rtc with current time
    rtc_init();
    block_until_rtc_updated();
    
    //Disconnect wifi
    disconnect_wifi();

    int ticks = 0;
    
    while (true)
    {
        sleep_ms(1000);
        printf("Ticks: {%d}\r\n", ticks);
        dht_sensor.measure();
        std::cout << dht_sensor.to_payload() << "\n";
        ticks++;

        sleep_ms(1000);

        if (ticks == 10) {
            ticks = 0;
            //Connect WIFI
            block_until_wifi_connected(ssid, pass);
    
            //Update rtc with current time
            rtc_init();
            block_until_rtc_updated();

            //Send data
            if (post_to_influx("https://alehem.eu:8086", "pico_dev" , "pico-dev", "123456", dht_sensor.to_payload())) {
                std::cout << "Sending data success!" << std::endl;
            } else {
                std::cout << "Failed to send data!" << std::endl;
            }

            //Disconnect wifi
            disconnect_wifi();
        }
    }
}
