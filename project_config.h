#ifndef __PROJECT_CONFIG_H__
#define __PROJECT_CONFIG_H__

#include <Arduino.h>

// #define SERIAL_LOG_ENABLE // Comment this to save power for actual deploy of the terminal

#define LORA_TERRESTRIAL_STATUS_UPLINK_INTERVAL_S (30 * 60) // 30 minutes
#define LORA_TERRESTRIAL_FREQUENCY_HZ 921400000
#define LORA_TERRESTRIAL_SPREADING_FACTOR RFT_LORA_SPREADING_FACTOR_9
#define LORA_TERRESTRIAL_BANDWIDTH RFT_LORA_BANDWIDTH_125KHZ

#define GNSS_DATA_UPDATE_INTERVAL_S (24 * 60 * 60) // 1 Day

#define MIN_PASS_ELAVATION 30 // Send to satellite pass with elavation > 30 degrees

#define LORA_SPACE_DELAY_BETWEEN_PACKET_S (15) // 15 Seconds

/* LoRa Terrestrial Configuration */
static uint8_t nwkS_key_terrestrial[] = {0xDA, 0x08, 0xA1, 0x10, 0xC6, 0xD5, 0x27, 0xB6, 0x21, 0x27, 0x5C, 0x44, 0xE0, 0xA7, 0x4B, 0xF1};
static uint8_t appS_key_terrestrial[] = {0xB5, 0x54, 0xD3, 0x9B, 0x4D, 0x4F, 0x8A, 0x69, 0x76, 0x44, 0xC4, 0x6D, 0x1B, 0x13, 0xF4, 0xF0};
static uint8_t dev_addr_terrestrial[] = {0x26, 0x0B, 0x87, 0x00};

static uint8_t nwkS_key_space[] = {0xDA, 0x08, 0xA1, 0x10, 0xC6, 0xD5, 0x27, 0xB6, 0x21, 0x27, 0x5C, 0x44, 0xE0, 0xA7, 0x4B, 0xF1};
static uint8_t appS_key_space[] = {0xB5, 0x54, 0xD3, 0x9B, 0x4D, 0x4F, 0x8A, 0x69, 0x76, 0x44, 0xC4, 0x6D, 0x1B, 0x13, 0xF4, 0xF0};
static uint8_t dev_addr_space[] = {0x26, 0x0B, 0x87, 0x00};

/* ON-BOARD GPIO DEFINE: RF210 */
#define LED_PIN PA0

#define GPS_ENABLE_PIN PA1
#define GPS_V_BACKUP_PIN PB5
#define GPS_TX_PIN PB6
#define GPS_RX_PIN PB7

#define LOG_TX_PIN PA2
#define LOG_RX_PIN PA3

#define VBAT_MEASURE_PIN PA15

/* INSIDE RAK3172 GPIO DEFINE */
#define SW_VCTL1_PIN PB8
#define SW_VCTL2_PIN PC13

/* MOCK GPS CONFIGURATION */
#define GPS_MOCK_COORDINATE_ENABLE
#define GPS_MOCK_LAT_VALUE 10870014ul
#define GPS_MOCK_LON_VALUE 106800479ul
#define GPS_MOCK_TIME_VALUE 1696259693ul // Monday, 2 October 2023 15:14:53 GMT

/* DEBUGGING */
// #define SERIAL_LOG_DEBUG_ENABLE // Uncomment this for printing debug log. Caution: This is in effect only if SERIAL_LOG_ENABLE is available.
// #define SYSTEM_SLEEP_DEBUG // Uncomment this for using delay instead of sleep

#if defined(SERIAL_LOG_DEBUG_ENABLE)
#warning SERIAL_LOG_DEBUG_ENABLE is active, this should be deactive to save power for actual deployment of the terminal.
#endif

#if defined(SYSTEM_SLEEP_DEBUG)
#warning SYSTEM_SLEEP_DEBUG is active, this should be deactive to save power for actual deployment of the terminal.
#endif

#endif /* __PROJECT_CONFIG_H__ */
