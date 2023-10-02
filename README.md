# RF210_LS_Test_for_RAK

The sketch example  shows how to communicate with a terrestrial gateway and Lacuna satellite.
ABP activation is used for both terrestrial and space communication.
For testing simplicity, two different sets of credentials (dev address and keys) are used for terrestrial and space communication respectively.


# Board Manager

Use ArduinoCore STM32 -> STM32WL series

https://github.com/stm32duino/Arduino_Core_STM32

# Librairies

The sketch needs several libraries : 

* <STM32LowPower.h> // https://github.com/stm32duino/STM32LowPower
* <STM32RTC.h>      // https://github.com/stm32duino/STM32RTC
* <RFThings.h>      // https://github.com/nguyenmanhthao996tn/LoRaSpaceLib-STM32WL
* <Sgp4.h>      // https://github.com/Hopperpop/Sgp4-Library
* <MicroNMEA.h> // http://librarymanager/All#MicroNMEA

# Configuration files 

tle.h has to be updated with recent TLE from the targeted satellite

Lorawan credentials are defined in project_config_h for terrestrial and space terminals.
