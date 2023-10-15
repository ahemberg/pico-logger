/**
 * Class for syncing the internal clock with the internet. 
 * This code is mostly taken from https://github.com/raspberrypi/pico-examples/blob/master/pico_w/wifi/ntp_client/picow_ntp_client.c
 */

#ifndef NTP_HPP
#define NTP_HPP

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <time.h>

#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/util/datetime.h"

#include "lwip/dns.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"

typedef struct NTP_T_ {
    ip_addr_t ntp_server_address;
    bool dns_request_sent;
    bool request_successful;
    struct udp_pcb *ntp_pcb;
    absolute_time_t ntp_test_time;
    alarm_id_t ntp_resend_alarm;
} NTP_T;

#define NTP_SERVER "pool.ntp.org"
#define NTP_MSG_LEN 48
#define NTP_PORT 123
#define NTP_DELTA 2208988800 // seconds between 1 Jan 1900 and 1 Jan 1970
#define NTP_TEST_TIME (30 * 1000)
#define NTP_RESEND_TIME (10 * 1000)

// Called with results of operation
static void ntp_result(NTP_T* state, int status, time_t *result);

static int64_t ntp_failed_handler(alarm_id_t id, void *user_data);

// Make an NTP request
static void ntp_request(NTP_T *state);

static int64_t ntp_failed_handler(alarm_id_t id, void *user_data);

// Call back with a DNS result
static void ntp_dns_found(const char *hostname, const ip_addr_t *ipaddr, void *arg);

// NTP data received
static void ntp_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

// Perform initialisation
NTP_T* ntp_init(void);

// Converts time_t to pico datetime_t
static datetime_t time_to_datetime(time_t *t);

// Blocks until internal RTC is updated or ntp call fails. Returns true if successful
bool update_rtc();

#endif