#include "main.h"

/****** VARIABLES: Event timestamp ******/
uint32_t current_timestamp = 0;

uint32_t gps_update_timestamp = 0;
uint32_t lora_terrestrial_status_uplink_timestamp = 0;

uint32_t lora_space_pass_start_timestamp = 0;
uint32_t lora_space_pass_duration_s = 0;

/****** VARIABLES: GNSS Data ******/
uint32_t gnss_latitude = 0;
uint32_t gnss_longtitude = 0;
uint32_t gnss_time = 0;


/****** FUNCTIONS: Main ******/
void setup(void)
{
  gpio_init();
  serial_init();
  lora_init();
  gnss_init();

  log("RF210_LS_Test_for_RAK\n");
  log("Initialization DONE\n");

  // Get GPS Data
  gnss_get_data(&gnss_latitude, &gnss_longtitude, &gnss_time);
  log("Get GPS Data DONE\n");
  log("Lat: %d\nLon: %d\nGPS Time: %d\n", gnss_latitude, gnss_longtitude, gnss_time);

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

  // DEBUG
  while (1);
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
    sat_predictor_get_next_pass(&lora_space_pass_start_timestamp, &lora_space_pass_duration_s);
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
