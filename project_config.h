#ifndef __PROJECT_CONFIG_H__
#define __PROJECT_CONFIG_H__

#define SERIAL_LOG_ENABLE // Comment this to save power for actual deploy of the terminal

#define LORA_TERRESTRIAL_STATUS_UPLINK_INTERVAL_S (30) // 30 Seconds
#define GNSS_DATA_UPDATE_INTERVAL_S (24*60*60) // 1 Day

#define LORA_SPACE_DELAY_BETWEEN_PACKET_S (15) // 15 Seconds

/* ON-BOARD GPIO DEFINE: RF210 */
#define LED_PIN PA0

#define GPS_ENABLE_PIN PA1
#define GPS_V_BACKUP_PIN PB5
#define GPS_TX_PIN PB6
#define GPS_RX_PIN PB7

#define LOG_TX_PIN PA2
#define LOG_RX_PIN PA3

#endif /* __PROJECT_CONFIG_H__ */
