#include <stdio.h>
#include <iostream>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/sleep.h"
#include "hardware/rtc.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hardware/rosc.h"
#include "hardware/structs/scb.h"
#include "secrets.hpp"
#include "sensor/dht22.hpp"
#include "network/ntp.hpp"


char ssid[] = SSID;
char pass[] = WPA_PASS;

time_t datetime_t_to_time_t(datetime_t t) {
    struct tm tm_time;

    tm_time.tm_year = t.year - 1900;
    tm_time.tm_mon = t.month -1;
    tm_time.tm_mday = t.day;
    tm_time.tm_hour = t.hour;
    tm_time.tm_min = t.min;
    tm_time.tm_sec = t.sec;
    tm_time.tm_isdst = -1;

    return mktime(&tm_time);
}

time_t get_time() {
    // TODO: assumes that the rtc is correclty set. In the time-utils class this should be verifiable
    datetime_t t;
    //Check RTC initialised
    rtc_get_datetime(&t);

    //Make a time_t from year month day and so on
    //Convert this to unix timestamp using standard c. Don't reinvent the wheel

    return datetime_t_to_time_t(t);
}

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


// static bool awake;

// static void sleep_callback(void) {
//     printf("RTC woke us up\n");
//     uart_default_tx_wait_blocking();
//     awake = true;
//     return;
// }

// static void rtc_sleep(int8_t seconds_to_sleep) {

//     time_t time_now = get_time();
//     time_t time_to_sleep_to = time_now + seconds_to_sleep; //TODO this is not guaranteed to work on all platformes

//     datetime_t t_alarm = time_to_datetime(time_to_sleep_to);

//     printf("Going to sleep.......\n");
//     uart_default_tx_wait_blocking();

//     sleep_goto_sleep_until(&t_alarm, &sleep_callback);
// }


void recover_from_sleep(uint scb_orig, uint clock0_orig, uint clock1_orig){

    //Re-enable ring Oscillator control
    rosc_write(&rosc_hw->ctrl, ROSC_CTRL_ENABLE_BITS);

    //reset procs back to default
    scb_hw->scr = scb_orig;
    clocks_hw->sleep_en0 = clock0_orig;
    clocks_hw->sleep_en1 = clock1_orig;

    //reset clocks
    clocks_init();
    stdio_init_all();

    return;
}


int main()
{

    Dht22 dht_sensor = Dht22("pico_dev", 15);

    stdio_init_all();

    //Neede for deep sleep
    uint scb_orig = scb_hw->scr;
    uint clock0_orig = clocks_hw->sleep_en0;
    uint clock1_orig = clocks_hw->sleep_en1;
    
    // RTC needed to keep track of time
    rtc_init();

    uint delay_backoff = 1000;
    while (!connect_wifi(ssid, pass)) {
        std::cout << "Failed toconnect to Wifi. Will retry in " << delay_backoff/1000 << " seconds" << std::endl;
        sleep_ms(delay_backoff);
        delay_backoff += 10000;
    }

    //Set RTC. Todo run this periodically.
    delay_backoff = 1;
    while (!update_rtc()) {
        std::cout << "Failed to update RTC. Will retry in " << delay_backoff/1000 << " seconds" << std::endl;
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

        //uart_default_tx_wait_blocking();
        //sleep_run_from_xosc();
        //std::cout << "Sleeping 10s" << std::endl;
        //rtc_sleep(10);
        //recover_from_sleep(scb_orig, clock0_orig, clock1_orig);
        //std::cout << "Awake!" << std::endl;
        sleep_ms(1000);
    }
}
