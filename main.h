#ifndef __MAIN_H__
#define __MAIN_H__

#include "project_config.h"
#include <Arduino.h>

/* Board sleep */
void system_sleep(uint32_t sleep_duration_s);

/* GNSS */
void gnss_init(void);
void gnss_get_data(uint32_t* gnss_latitude, uint32_t* gnss_longtitude, uint32_t* gnss_time);

/* LoRa */
void lora_init(void);
void lora_send_terrestrial_status_uplink(void);
void lora_send_space_uplink(void);

/* Satellite pass predictor */
void sat_predictor_init(void);
void sat_predictor_get_next_pass(uint32_t* pass_start_timestamp, uint32_t* pass_duration_s);

/* Serial */
void serial_init(void);
void log(char * fmt, ...);

/* Others */
void gpio_init(void);

void led_blink(uint8_t num_of_blinks);
void led_on(void);
void led_off(void);

void event_timestamp_calibration(uint32_t* gps_update_timestamp, uint32_t* lora_terrestrial_status_uplink_timestamp, uint32_t lora_space_pass_start_timestamp, uint32_t lora_space_pass_duration_s);

uint32_t smallest(uint32_t val1, uint32_t val2 = 0xFFFFFFFF, uint32_t val3 = 0xFFFFFFFF);

#endif /* __MAIN_H__ */
