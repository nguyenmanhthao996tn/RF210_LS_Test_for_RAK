#include "main.h"

HardwareSerial SerialGPS(USART1);
STM32RTC &rtc = STM32RTC::getInstance();

char nmeaBuffer[100];
MicroNMEA nmea(nmeaBuffer, sizeof(nmeaBuffer));

// Satellite predictor
Sgp4 predictor;

rfthings_sx126x subghz_inst;
extern uint8_t nwkS_key_terrestrial[];
extern uint8_t appS_key_terrestrial[];
extern uint8_t dev_addr_terrestrial[];
extern uint8_t nwkS_key_space[];
extern uint8_t appS_key_space[];
extern uint8_t dev_addr_space[];

extern char satname[];
extern char tle_line1[];
extern char tle_line2[];

static unsigned long unixTimestamp(int year, int month, int day, int hour, int min, int sec)
{
  const short days_since_beginning_of_year[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
  int leap_years = ((year - 1) - 1968) / 4 - ((year - 1) - 1900) / 100 + ((year - 1) - 1600) / 400;
  long days_since_1970 = (year - 1970) * 365 + leap_years + days_since_beginning_of_year[month - 1] + day - 1;
  if ((month > 2) && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)))
    days_since_1970 += 1; /* +leap day, if year is a leap year */
  return sec + 60 * (min + 60 * (hour + 24 * days_since_1970));
}

void system_init(void)
{
  rtc.begin();
}

void system_sleep(uint32_t sleep_duration_s)
{
#warning No implement yet
}

void gnss_init(void)
{
  pinMode(GPS_ENABLE_PIN, OUTPUT);
  pinMode(GPS_V_BACKUP_PIN, OUTPUT);

#if !defined(GPS_MOCK_COORDINATE_ENABLE) // Normal operation
  digitalWrite(GPS_ENABLE_PIN, HIGH);
  digitalWrite(GPS_V_BACKUP_PIN, HIGH);

  delay(500);

  SerialGPS.setTx(GPS_TX_PIN);
  SerialGPS.setRx(GPS_RX_PIN);
  SerialGPS.begin(9600, SERIAL_8N1);

  while (!SerialGPS)
    ;

  while (!SerialGPS.available())
    ;

  digitalWrite(GPS_ENABLE_PIN, LOW);

  SerialGPS.flush();
#else // Mock GPS coordinates
  // Nothing to do here
#endif
}

void gnss_get_data(uint32_t *gnss_latitude, uint32_t *gnss_longtitude, uint32_t *gnss_time)
{
#if !defined(GPS_MOCK_COORDINATE_ENABLE) // Normal operation
#warning Need testing to verify all functionalities
  digitalWrite(GPS_ENABLE_PIN, HIGH);

  delay(1000);

  SerialGPS.flush();
  nmea.clear();

  char received_char;
  uint32_t last_blink = millis();
  for (;;)
  {
    // Get the character from GPS Serial & process it
    if (SerialGPS.available())
    {
      received_char = SerialGPS.read();
      nmea.process(received_char);
    }

    // Check read exit condition
    if (nmea.isValid() && (nmea.getNumSatellites() > 4))
    {
      break;
    }

    // Blink LED every second
    if (millis() - last_blink > 1000)
    {
      last_blink = millis();
      led_blink(1);
    }
  }

  *gnss_latitude = nmea.getLatitude();
  *gnss_longtitude = nmea.getLongitude();
  *gnss_time = unixTimestamp(nmea.getYear(), nmea.getMonth(), nmea.getDay(), nmea.getHour(), nmea.getMinute(), nmea.getSecond());

  digitalWrite(GPS_ENABLE_PIN, HIGH);
#else // Mock GPS coordinates
  *gnss_latitude = GPS_MOCK_LAT_VALUE;
  *gnss_longtitude = GPS_MOCK_LON_VALUE;
  *gnss_time = GPS_MOCK_TIME_VALUE;
#endif

  // Update time to RTC module
  rtc.setEpoch((time_t)(*gnss_time));
  // rtc.setTime(21, 32, 05); // Hour, Min, Sec
  // rtc.setDate(01, 10, 23); // Day, Month, Year (actual year - 2000)
}

static void sw_ctrl_set_mode(bool mode_tx)
{
  if (mode_tx)
  {
    digitalWrite(SW_VCTL1_PIN, LOW);
    digitalWrite(SW_VCTL2_PIN, HIGH);
  }
  else
  {
    digitalWrite(SW_VCTL1_PIN, HIGH);
    digitalWrite(SW_VCTL2_PIN, LOW);
  }
}

static void sw_ctrl_set_mode_tx(void)
{
  sw_ctrl_set_mode(true);
}

static void sw_ctrl_set_mode_rx(void)
{
  sw_ctrl_set_mode(false);
}

void lora_init(void)
{
  rft_status_t status;

  pinMode(SW_VCTL1_PIN, OUTPUT);
  pinMode(SW_VCTL2_PIN, OUTPUT);

  status = subghz_inst.init(RFT_REGION_EU863_870);
  if (status != RFT_STATUS_OK)
  {
    return;
  }
}

uint8_t build_payload(uint8_t *buffer, bool send_to_space, uint32_t gps_lattitude, uint32_t gps_longtitude, uint32_t next_pass_start, uint32_t next_pass_duration, uint32_t next_gps_update)
{
  if (buffer == NULL)
    return 0;

  /****** What should we send? ******/
  // Packet type: 1 = send to space, 0 = send to terrestrial gateway
  buffer[0] = send_to_space ? 1 : 0;

  // Packet build time
  uint32_t now = rtc.getEpoch();
  buffer[1] = (now >> 24) & 0xFF;
  buffer[2] = (now >> 16) & 0xFF;
  buffer[3] = (now >> 8) & 0xFF;
  buffer[4] = now & 0xFF;

  // Last coordinates
  buffer[5] = (gps_lattitude >> 24) & 0xFF;
  buffer[6] = (gps_lattitude >> 16) & 0xFF;
  buffer[7] = (gps_lattitude >> 8) & 0xFF;
  buffer[8] = gps_lattitude & 0xFF;
  buffer[9] = (gps_lattitude >> 24) & 0xFF;
  buffer[10] = (gps_lattitude >> 16) & 0xFF;
  buffer[11] = (gps_lattitude >> 8) & 0xFF;
  buffer[12] = gps_lattitude & 0xFF;

  // Next satellite pass start & duration
  buffer[13] = (next_pass_start >> 24) & 0xFF;
  buffer[14] = (next_pass_start >> 16) & 0xFF;
  buffer[15] = (next_pass_start >> 8) & 0xFF;
  buffer[16] = next_pass_start & 0xFF;

  buffer[17] = (next_pass_duration >> 24) & 0xFF;
  buffer[18] = (next_pass_duration >> 16) & 0xFF;
  buffer[19] = (next_pass_duration >> 8) & 0xFF;
  buffer[20] = next_pass_duration & 0xFF;

  // Next GPS Update timestamp
  buffer[21] = (next_gps_update >> 24) & 0xFF;
  buffer[22] = (next_gps_update >> 16) & 0xFF;
  buffer[23] = (next_gps_update >> 8) & 0xFF;
  buffer[24] = next_gps_update & 0xFF;

  return 25;
}

void lora_send_terrestrial_status_uplink(uint8_t *payload, uint8_t payload_len)
{
  // Set parameters
  // LoRaWAN parameters
  subghz_inst.set_lorawan_activation_type(RFT_LORAWAN_ACTIVATION_TYPE_ABP);
  subghz_inst.set_application_session_key(appS_key_terrestrial);
  subghz_inst.set_network_session_key(nwkS_key_terrestrial);
  subghz_inst.set_device_address(dev_addr_terrestrial);

  subghz_inst.set_tx_port(1);
  subghz_inst.set_rx1_delay(1000);

  // LoRa parameters
  subghz_inst.set_tx_power(22);
  subghz_inst.set_frequency(868100000);
  subghz_inst.set_spreading_factor(RFT_LORA_SPREADING_FACTOR_9);
  subghz_inst.set_bandwidth(RFT_LORA_BANDWIDTH_125KHZ);
  subghz_inst.set_coding_rate(RFT_LORA_CODING_RATE_4_6);
  subghz_inst.set_syncword(RFT_LORA_SYNCWORD_PUBLIC);

  // Send packet
  rft_status_t status = subghz_inst.send_uplink((byte *)payload, payload_len, sw_ctrl_set_mode_tx, sw_ctrl_set_mode_rx);
  if (status != RFT_STATUS_OK)
  {
    return;
  }
}

void lora_send_space_uplink(uint8_t *payload, uint8_t payload_len)
{
  // Set parameters
  // LoRaWAN parameters
  subghz_inst.set_lorawan_activation_type(RFT_LORAWAN_ACTIVATION_TYPE_ABP);
  subghz_inst.set_application_session_key(appS_key_space);
  subghz_inst.set_network_session_key(nwkS_key_space);
  subghz_inst.set_device_address(dev_addr_space);

  // Config LR-FHSS parameter
  subghz_inst.set_lrfhss_codingRate(RFT_LRFHSS_CODING_RATE_1_3);
  subghz_inst.set_lrfhss_bandwidth(RFT_LRFHSS_BANDWIDTH_136_7_KHZ);
  subghz_inst.set_lrfhss_grid(RFT_LRFHSS_GRID_3_9_KHZ);
  subghz_inst.set_lrfhss_hopping(true);
  subghz_inst.set_lrfhss_nbSync(4);
  subghz_inst.set_lrfhss_frequency(868200000);
  subghz_inst.set_lrfhss_power(22);
  subghz_inst.set_lrfhss_syncword(0x2C0F7995);

  // Send packet
  rft_status_t status = subghz_inst.send_lorawan_over_lrfhss((byte *)payload, payload_len, sw_ctrl_set_mode_tx);
  if (status != RFT_STATUS_OK)
  {
    return;
  }
}

void sat_predictor_init(void)
{
#warning No implement yet
}

void sat_predictor_get_next_pass(uint32_t *pass_start_timestamp, uint32_t *pass_duration_s, uint32_t gnss_latitude, uint32_t gnss_longtitude)
{
  double lat = (double)(gnss_latitude / 1.0e6);
  double lon = (double)(gnss_longtitude / 1.0e6);
  double alt = 0;
  predictor.site(lat, lon, alt);

  predictor.init(satname, tle_line1, tle_line2);

  passinfo overpass;                            // structure to store overpass info
  predictor.initpredpoint((unsigned long)rtc.getEpoch(), 0.0); // finds the startpoint

  bool good_pass_found = false;
  bool predict_result;
  for (uint8_t i = 0; i < 15; i++) // Search for the next 15 pass for a good max elavtion
  {
    predict_result = predictor.nextpass(&overpass, 20); // search for the next overpass, if there are more than 20 maximums below the horizon it returns false
    if (predict_result && (overpass.maxelevation > MIN_PASS_ELAVATION))
    {
      good_pass_found = true;
      break; // Stop the prediction iteration
    }
    else
    {
    }
  }

  log("Predict next satellite pass\n");
  if (good_pass_found)
  {
    uint32_t next_satellite_pass_start = getUnixFromJulian(overpass.jdstart);
    uint32_t next_satellite_pass_stop = getUnixFromJulian(overpass.jdstop);

    // Add 5 minutes margin
    *pass_start_timestamp = next_satellite_pass_start - (5 * 60);
    *pass_duration_s = (next_satellite_pass_stop - next_satellite_pass_start) + (10 * 60);

    log("A good pass found at");
    log("\n\tStart: %ld\n\tStop: %ld\n\tMax. Elavation:  %f\n", next_satellite_pass_start, next_satellite_pass_stop, overpass.maxelevation);
  }
  else
  {
    *pass_start_timestamp = 0;
    *pass_duration_s = 0;

    log("Pass NOT found\n");
  }
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
