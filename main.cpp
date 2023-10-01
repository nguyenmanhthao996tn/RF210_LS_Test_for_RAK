#include "main.h"

HardwareSerial SerialGPS(USART1);

void system_sleep(uint32_t sleep_duration_s)
{
#warning No implement yet
}

void gnss_init(void)
{
  pinMode(GPS_ENABLE_PIN, OUTPUT);
  digitalWrite(GPS_ENABLE_PIN, HIGH);
  pinMode(GPS_V_BACKUP_PIN, OUTPUT);
  digitalWrite(GPS_V_BACKUP_PIN, HIGH);

  delay(500);

  SerialGPS.setTx(GPS_TX_PIN);
  SerialGPS.setRx(GPS_RX_PIN);
  SerialGPS.begin(9600, SERIAL_8N1);

  while (!SerialGPS);

  while (!SerialGPS.available());
}

void gnss_get_data(uint32_t *gnss_latitude, uint32_t *gnss_longtitude, uint32_t *gnss_time)
{
#warning No implement yet
  SerialGPS.flush();
}

void lora_init(void)
{
#warning No implement yet
}

void lora_send_terrestrial_status_uplink(void)
{
#warning No implement yet
}

void lora_send_space_uplink(void)
{
#warning No implement yet
}

void sat_predictor_init(void)
{
#warning No implement yet
}

void sat_predictor_get_next_pass(uint32_t *pass_start_timestamp, uint32_t *pass_duration_s)
{
#warning No implement yet
}

void serial_init(void)
{
#if defined(SERIAL_LOG_ENABLE)
  Serial.setTx(LOG_TX_PIN);
  Serial.setRx(LOG_RX_PIN);
  Serial.begin(115200);
  while (!Serial)
    ;
#endif
}

void log(char * msg)
{
#if defined(SERIAL_LOG_ENABLE)
  Serial.print(msg);
#endif
}

void gpio_init(void)
{
  // LED
  pinMode(LED_PIN, OUTPUT);
  led_off();
}

void led_blink(uint8_t num_of_blinks, uint32_t led_on_duration_ms, uint32_t led_off_duration_ms)
{
  for (; num_of_blinks > 0; num_of_blinks--)
  {
    led_on();
    delay(led_on_duration_ms);

    led_off();
    delay(led_off_duration_ms);
  }
}

void led_on(void)
{
  digitalWrite(LED_PIN, HIGH);
}

void led_off(void)
{
  digitalWrite(LED_PIN, LOW);
}

void event_timestamp_calibration(uint32_t *gps_update_timestamp, uint32_t *lora_terrestrial_status_uplink_timestamp, uint32_t lora_space_pass_start_timestamp, uint32_t lora_space_pass_duration_s)
{
#define SAT_PASS_GUARD_TIME (15 * 60) // No GPS Update or Status uplink in a satellite pass or 15 mins before it

  const uint32_t start = lora_space_pass_start_timestamp - SAT_PASS_GUARD_TIME;
  const uint32_t stop = lora_space_pass_start_timestamp + lora_space_pass_duration_s;

  if ((start < *gps_update_timestamp) && (*gps_update_timestamp < stop))
  {
    *gps_update_timestamp = stop + (random(5, 15) * 60); // Reschedule from 5 to 15 mins after a pass
  }

  if ((start < *lora_terrestrial_status_uplink_timestamp) && (*lora_terrestrial_status_uplink_timestamp < stop))
  {
    *lora_terrestrial_status_uplink_timestamp = stop + (random(1, 5) * 60); // Reschedule from 1 to 5 mins after a pass
  }
}

uint32_t smallest(uint32_t val1, uint32_t val2, uint32_t val3)
{
  uint32_t smallest_value = val1;

  if (val2 < smallest_value)
    smallest_value = val2;
  if (val3 < smallest_value)
    smallest_value = val3;

  return smallest_value;
}
