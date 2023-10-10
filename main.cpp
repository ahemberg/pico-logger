#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/cyw43_arch.h"
#include "secrets.hpp"

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
    stdio_init_all();

    if (!connect_wifi(ssid, pass)) {
        return 1;
    }
    
    int ticks = 0;

    /* code */
    while (true)
    {
        sleep_ms(1000);
        printf("Ticks: {%d}\r\n", ticks);
        ticks++;
        sleep_ms(1000);
    }
}
