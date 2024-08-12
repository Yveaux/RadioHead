// stm32wlx_client.ino
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the STM32WLx class. STM32WLx class does not provide for addressing or
// reliability, so you should only use on its own RH_SX126X if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example stm32wlx_server
// (dont forget to make sure the madulation schemes and frequencies are the same).
// In order to use the Arduino IDE to program the Wio-E5, you must
// install the stm32duino package using these instructions:
//   https://community.st.com/t5/stm32-mcus/stm32-arduino-stm32duino-tutorial/ta-p/49649
// Tested with Wio-E5 mini. In Arduino, select:
//   Tools -> Board -> STM32 MCU based boards -> LoRa boards
//   Tools -> Board part number -> LoRa-E5 mini
//   Tools -> U(S)ART support -> Enabled (generic Serial)

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
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  // You can change the frequency:
  // driver.setFrequency(868.0);

  // You can change the modulation parameters with eg
  // driver.setModemConfig(RH_SX126X::Bw500Cr45Sf128);
  
  // You can change the power level in dBm
//  driver.setTxPower(14);
}

void loop()
{
  Serial.println("Sending to stm32wlx_server");
  // Send a message to stm32wlx_server
  uint8_t data[] = "Hello World!";
  driver.send(data, sizeof(data));
  
  driver.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_SX126x_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (driver.waitAvailableTimeout(3000))
  { 
    // Should be a reply message for us now   
    if (driver.recv(buf, &len))
   {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
//      Serial.print("RSSI: ");
//      Serial.println(driver.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is stm32wlx_server running?");
  }
  delay(400);
}
