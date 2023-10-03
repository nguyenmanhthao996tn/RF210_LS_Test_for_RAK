# project_config.h

This page describes configurations defined in the ```project_config.h``` file.

---

### SERIAL_LOG_ENABLE

Enable print the log to Serial / USART. Comment this line to disable.

Disable this feature will also disable [SERIAL_LOG_DEBUG_ENABLE](#serial_log_debug_enable).

**This feature should be disable for actual deployment to reduce board power consumption.**

### LORA_TERRESTRIAL_STATUS_UPLINK_INTERVAL_S

Time interval between status uplink packets send to terrestrial gateway. This value is defined in second(s).

### LORA_TERRESTRIAL_FREQUENCY_HZ

Frequency of uplink packets send to terrestrial gateway. This value is defined in Hertz (Hz).

Normally, this configuration meets the [RP002-1.0.4 Regional Parameters](https://resources.lora-alliance.org/technical-specifications/rp002-1-0-4-regional-parameters) by LoRa Alliance. If you have issue in sending terrestrial uplink, **consult to your local gateway manager for the exact configuration in your area.**

### LORA_TERRESTRIAL_SPREADING_FACTOR

The spreading factor of uplink packets send to terrestrial gateway. Acceptable values are list in the following table:

|     **Spreading factor**     |
|:----------------------------:|
|  RFT_LORA_SPREADING_FACTOR_7 |
|  RFT_LORA_SPREADING_FACTOR_8 |
|  RFT_LORA_SPREADING_FACTOR_9 |
| RFT_LORA_SPREADING_FACTOR_10 |
| RFT_LORA_SPREADING_FACTOR_11 |
| RFT_LORA_SPREADING_FACTOR_12 |

Normally, this configuration meets the [RP002-1.0.4 Regional Parameters](https://resources.lora-alliance.org/technical-specifications/rp002-1-0-4-regional-parameters) by LoRa Alliance. If you have issue in sending terrestrial uplink, **consult to your local gateway manager for the exact configuration in your area.**

### LORA_TERRESTRIAL_BANDWIDTH

The bandwidth of uplink packets send to terrestrial gateway. Acceptable values are list in the following table:

|        **Bandwidth**       |
|:--------------------------:|
|  RFT_LORA_BANDWIDTH_125KHZ |
|  RFT_LORA_BANDWIDTH_250KHZ |

Normally, this configuration meets the [RP002-1.0.4 Regional Parameters](https://resources.lora-alliance.org/technical-specifications/rp002-1-0-4-regional-parameters) by LoRa Alliance. If you have issue in sending terrestrial uplink, **consult to your local gateway manager for the exact configuration in your area.**

### GNSS_DATA_UPDATE_INTERVAL_S

Time interval to renew GPS data including device's latitude, longtitude & time. This value is defined in second(s).

### MIN_PASS_ELAVATION

The smallest max. elevation that the terminal / device will wake up to send data to the satellite. This value is defined in degree(s).

*The closer to 90 degrees max. elevation, the better the pass will be. However, it also means the less chance to send data.*

### LORA_SPACE_DELAY_BETWEEN_PACKET_S

The delay between packets sent to the satellite. This value is defined in second(s).

### GPS_MOCK_COORDINATE_ENABLE

Enable using faked fixed GPS data includes [latitude](#gps_mock_lat_value), [longtitude](#gps_mock_lon_value), and [time](#gps_mock_time_value).

### GPS_MOCK_LAT_VALUE

Faked fix latitude value. This value is defined in millionths of a degree. (Divide this value by 1e6 to get the actual latitude)

### GPS_MOCK_LON_VALUE

Faked fix longtitude value. This value is defined in millionths of a degree. (Divide this value by 1e6 to get the actual latitude)

### GPS_MOCK_TIME_VALUE

Faked fix time value. This value is the Unix EPOCH time. It's number of seconds since January 1, 1970 (midnight UTC/GMT).

### SERIAL_LOG_DEBUG_ENABLE

Enable print the debug log to Serial / USART. Comment this line to disable. The printed log will include "[DEBUG]" at the beginning of the line.

This feature will also be disabled if [SERIAL_LOG_ENABLE](#serial_log_enable) disabled.

**This feature should be disable for actual deployment to reduce board power consumption.**

### SYSTEM_SLEEP_DEBUG

Enable this will replace board deep sleep with normal delay. This should be used for debugging only. Comment this line to disable

---
