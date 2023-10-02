#include "main.h"

/****** VARIABLES: Event timestamp ******/
uint32_t current_timestamp = 0;

uint32_t gps_update_timestamp = 0;
uint32_t lora_terrestrial_status_uplink_timestamp = 0;

uint32_t lora_space_pass_start_timestamp = 0;
uint32_t lora_space_pass_duration_s = 0;

/****** VARIABLES: GNSS Data ******/
int32_t gnss_latitude = 0;
int32_t gnss_longtitude = 0;
uint32_t gnss_time = 0;

extern STM32RTC &rtc;

/****** FUNCTIONS: Main ******/
void setup(void)
{
  system_init();
  gpio_init();
  serial_init();
  lora_init();
  gnss_init();

  log("RF210_LS_Test_for_RAK\n");
  log("Initialization DONE\n");

  // Get GPS Data
  gnss_get_data(&gnss_latitude, &gnss_longtitude, &gnss_time);
  gps_update_timestamp = rtc.getEpoch() + GNSS_DATA_UPDATE_INTERVAL_S;
  log("Get GPS Data DONE\n");
  log("\tLat: %d\n\tLon: %d\n\tGPS Time: %u\n", gnss_latitude, gnss_longtitude, gnss_time);
  log("\tLong date format: %02d/%02d/%02d ", rtc.getDay(), rtc.getMonth(), rtc.getYear());
  log("%02d:%02d:%02d.%03u\n", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getSubSeconds());
  log("\tNext GPS update: %u\n", gps_update_timestamp);

  // Calculate the next pass
  sat_predictor_get_next_pass(&lora_space_pass_start_timestamp, &lora_space_pass_duration_s, gnss_latitude, gnss_longtitude);
  event_timestamp_calibration(&gps_update_timestamp, &lora_terrestrial_status_uplink_timestamp, lora_space_pass_start_timestamp, lora_space_pass_duration_s);
  log("Predict the next satellite pass DONE\n");
  log("\tPass start: %u\n\tPass duration: %u\n", lora_space_pass_start_timestamp, lora_space_pass_duration_s);

  // Send stutus packet
  led_blink(2);
  uint8_t payload[32];
  uint8_t payload_len = build_payload(payload, false, gnss_latitude, gnss_longtitude, lora_space_pass_start_timestamp, lora_space_pass_duration_s, gps_update_timestamp);
  lora_send_terrestrial_status_uplink(payload, payload_len);
  lora_send_terrestrial_status_uplink(payload, payload_len);
  log("Send terrestrial status uplink DONE\n");

  // Turn on the LED 1 second to indicate initialization DONE
  led_on();
  delay(1000);
  led_off();

  current_timestamp = millis();

  while (1)
    ; // DEBUG: We stop the debugging here
}

void loop(void)
{
  if (current_timestamp > lora_space_pass_start_timestamp)
  {
    // Send uplink while satellite pass still in it's duration
    while (millis() < (lora_space_pass_start_timestamp + lora_space_pass_duration_s))
    {
      // Send
      led_blink(2);
      uint8_t payload[32];
      uint8_t payload_len = build_payload(payload, true, gnss_latitude, gnss_longtitude, lora_space_pass_start_timestamp, lora_space_pass_duration_s, gps_update_timestamp);
      lora_send_space_uplink(payload, payload_len);

      // Delay
      system_sleep(LORA_SPACE_DELAY_BETWEEN_PACKET_S);
    }

    // Calculate the next pass
    sat_predictor_get_next_pass(&lora_space_pass_start_timestamp, &lora_space_pass_duration_s, gnss_latitude, gnss_longtitude);
  }

  if (current_timestamp > gps_update_timestamp)
  {
    // Get GNSS
    gnss_get_data(&gnss_latitude, &gnss_longtitude, &gnss_time);

    // Calculate the next timestamp
    gps_update_timestamp += GNSS_DATA_UPDATE_INTERVAL_S;
  }

  if (current_timestamp > lora_terrestrial_status_uplink_timestamp)
  {
    // Send status uplink
    led_blink(3);
    uint8_t payload[32];
    uint8_t payload_len = build_payload(payload, false, gnss_latitude, gnss_longtitude, lora_space_pass_start_timestamp, lora_space_pass_duration_s, gps_update_timestamp);
    lora_send_terrestrial_status_uplink(payload, payload_len);

    // Calculate the next timestamp
    lora_terrestrial_status_uplink_timestamp += LORA_TERRESTRIAL_STATUS_UPLINK_INTERVAL_S;
  }

  // Check & Calibration event timestamps
  event_timestamp_calibration(&gps_update_timestamp, &lora_terrestrial_status_uplink_timestamp, lora_space_pass_start_timestamp, lora_space_pass_duration_s);

  // Go to sleep until next event
  system_sleep(smallest(gps_update_timestamp, lora_terrestrial_status_uplink_timestamp, lora_space_pass_start_timestamp));
}
