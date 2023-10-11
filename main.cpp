#include <stdio.h>
#include <iostream>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/cyw43_arch.h"
#include "secrets.hpp"
#include "sensor/dht22.hpp"

char ssid[] = SSID;
char pass[] = WPA_PASS;


bool connect_wifi(char* ssid, char* pass) {
 if (cyw43_arch_init_with_country(CYW43_COUNTRY_UK))
    {
        printf("failed initialize\n");
        return false;
    }
    printf("initialized\n");

    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms(ssid, pass, CYW43_AUTH_WPA2_AES_PSK, 10000))
    {
        printf("failed to connect\n");
        return false;
    }
    printf("connected\n");
    return true;
}


int main()
{

    Dht22 dht_sensor = Dht22("pico_dev");

    stdio_init_all();

    uint delay_backoff = 1000;
    while (!connect_wifi(ssid, pass)) {
        std::cout << "Failed to connect to wifi. Trying again in " << delay_backoff/1000 << "seconds" << std::endl;
        sleep_ms(delay_backoff);
        delay_backoff += 10000;
    }
    
    int ticks = 0;

    /* code */
    while (true)
    {
        sleep_ms(1000);
        printf("Ticks: {%d}\r\n", ticks);
        dht_sensor.measure();
        std::cout << dht_sensor.to_payload() << "\n";
        ticks++;
        sleep_ms(1000);
    }
}
