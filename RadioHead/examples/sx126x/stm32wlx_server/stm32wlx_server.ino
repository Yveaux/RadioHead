// stm32wlx_server.ino
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_SX126X class. RH_SX126X class does not provide for addressing or
// reliability, so you should only use on its own RH_SX126X if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example stm32wlx_client or rf95_client
// (dont forget to make sure the madulation schemes and frequencies are the same).
//
// In order to use the Arduino IDE to program the Wio-E5, you must
// install the stm32duino package using these instructions:
//   https://community.st.com/t5/stm32-mcus/stm32-arduino-stm32duino-tutorial/ta-p/49649
// Tested with Wio-E5 mini. In Arduino, select:
//   Tools -> Board -> STM32 MCU based boards -> LoRa boards
//   Tools -> Board part number -> LoRa-E5 mini
//   Tools -> U(S)ART support -> Enabled (generic Serial)

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

  // Defaults after init are 915.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  // You can change the frequency:
  //driver.setFrequency(868.0);

  // You can change the modulation parameters with eg
  //driver.setModemConfig(RH_SX126x::LoRa_Bw500Cr45Sf128);
  
  // You can change the power level in dBm (-9 to +22 on the Wio-E5 mini)
  // Default is 13dBm
  //driver.setTxPower(0);
}

void loop()
{
 if (driver.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_SX126x_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (driver.recv(buf, &len))
    {
//      RH_SX126X::printBuffer("request: ", buf, len);
      Serial.print("got request: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(driver.lastRssi(), DEC);
      
      // Send a reply
      uint8_t data[] = "And hello back to you";
      driver.send(data, sizeof(data));
      driver.waitPacketSent();
      Serial.println("Sent a reply");
    }
    else
    {
      Serial.println("recv failed");
    }
  }
}


