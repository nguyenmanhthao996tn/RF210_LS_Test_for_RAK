#ifndef __MAIN_H__
#define __MAIN_H__

#include "project_config.h"
#include <Arduino.h>
#include <RFThings.h>
#include <radio/sx126x/rfthings_sx126x.h>

/*********** Board sleep ***********/
void system_sleep(uint32_t sleep_duration_s);

/*********** GNSS ***********/
/**
 * @brief (Blocking) Initialization of GNSS module & the Serial module communicating with it.
 * 
 */
void gnss_init(void);

/**
 * @brief (Blocking) Update the latest GNSS data. Divide returned Longtitude & Latitude to 1e7 to get actual values.
 * 
 * @param gnss_latitude Pointer to Latitude variable to be updated
 * @param gnss_longtitude Pointer to Longtitude variable to be updated
 * @param gnss_time Pointer to time variable to be updated
 */
void gnss_get_data(uint32_t* gnss_latitude, uint32_t* gnss_longtitude, uint32_t* gnss_time);

/*********** LoRa ***********/
void lora_init(void);
uint8_t build_payload(uint8_t* buffer, bool send_to_space, uint32_t gps_lattitude, uint32_t gps_longtitude, uint32_t next_pass_start, uint32_t next_pass_duration, uint32_t next_gps_update);
void lora_send_terrestrial_status_uplink(uint8_t* payload, uint8_t payload_len);
void lora_send_space_uplink(uint8_t* payload, uint8_t payload_len);

/*********** Satellite pass predictor ***********/
void sat_predictor_init(void);
void sat_predictor_get_next_pass(uint32_t* pass_start_timestamp, uint32_t* pass_duration_s);

/*********** Serial ***********/

/**
 * @brief Initialization of Serial module. In this demo, it's mainly
 * used for print log messages. Printing to log could be disabled
 * by undefined SERIAL_LOG_ENABLE.
 * 
 */
void serial_init(void);

/**
 * @brief Print log message. Printing
 * to log could be disabled by undefined SERIAL_LOG_ENABLE.
 * 
 * @param msg Your message to print
 */
#if defined(SERIAL_LOG_ENABLE)
#define log( ... ) { \
  char log_buffer[128]; \
  memset(log_buffer, 0, 128); \
  snprintf(log_buffer, 128, __VA_ARGS__); \
  Serial.print(log_buffer); \
 }
 #else
 log( ... )
 #endif

/*********** Others ***********/
void gpio_init(void);

/**
 * @brief (Non-blocking) Blink the on-board LED
 * 
 * @param num_of_blinks Number of blink times
 * @param led_on_duration_ms Duration of LED on in milliseconds. Default = 50 milliseconds
 * @param led_off_duration_ms Duration of LED off in milliseconds. Default = 150 milliseconds
 */
void led_blink(uint8_t num_of_blinks, uint32_t led_on_duration_ms = 50, uint32_t led_off_duration_ms = 150);

/**
 * @brief (Non-blocking) Turn on-board LED on
 * 
 */
void led_on(void);

/**
 * @brief (Non-blocking) Turn on-board LED off
 * 
 */
void led_off(void);

/**
 * @brief (Non-blocking) Calibrate GPS update & terrestrial uplink event timestamp if they are too close to a satellite pass.
 * 
 * @param gps_update_timestamp GPS Update timestamp
 * @param lora_terrestrial_status_uplink_timestamp LoRa Terrestrial status uplink timestamp
 * @param lora_space_pass_start_timestamp The next satellite pass start timestamp
 * @param lora_space_pass_duration_s The next satellite pass duration in seconds
 */
void event_timestamp_calibration(uint32_t* gps_update_timestamp, uint32_t* lora_terrestrial_status_uplink_timestamp, uint32_t lora_space_pass_start_timestamp, uint32_t lora_space_pass_duration_s);

/**
 * @brief (Non-blocking) Get the smallest values among uint32_t values (3 max).
 * 
 * @param val1 Value 1
 * @param val2 Value 2
 * @param val3 Value 3
 * @return uint32_t Smallest number
 */
uint32_t smallest(uint32_t val1, uint32_t val2 = 0xFFFFFFFF, uint32_t val3 = 0xFFFFFFFF);

#endif /* __MAIN_H__ */
