// stm32wlx_continuous.ino
// -*- mode: C++ -*-
// Example sketch showing how to start a continuous carrier wave transmission
// for measuring power output. CAUTION: it may be illegal for you to transmit continuously
// without a dummy load.
//
// In order to use the Arduino IDE to program the Wio-E5, you must
// install the stm32duino package using these instructions:
//   https://community.st.com/t5/stm32-mcus/stm32-arduino-stm32duino-tutorial/ta-p/49649
// Tested with Wio-E5 mini. In Arduino, select:
//   Tools -> Board -> STM32 MCU based boards -> LoRa boards
//   Tools -> Board part number -> LoRa-E5 mini
//   Tools -> U(S)ART support -> Enabled (generic Serial)
// Caution: it is probably possible to damage your chip by setting high power levels
// without a suitable RF termination.

#include <SPI.h>
#include <RH_STM32WLx.h>

// Single instance of the driver. The defaults will work for Seed WiO-E5 mini board, and other boards 
// equipped with Seeed LoRa-E5 modules, such as NUCLEO_WL55JC1
RH_STM32WLx driver;

void setup() 
{
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available

  if (!driver.init())
    Serial.println("init failed");  

  // You can change the frequency:
  driver.setFrequency(868.0); // Specs are at this frequency

  // You can change the power level in dBm(-9 to +22 on the Wio-E5 mini)
  // Default is 13dBm
  driver.setTxPower(0);
  driver.setTxContinuous(); // Send continuous carrier wave at the power and frequency set above
}

void loop()
{
  return;
}


