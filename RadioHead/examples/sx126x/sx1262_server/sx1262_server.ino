// sx1262_server.ino
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_SX126x class and a basic SX1262 module connected to an Arduino compatible processor
// It is designed to work with the examples stm32wlx_client and sx1262_client.
// Tested with G-Nice LoRa1262-915 and Teensy 3.1

#include <SPI.h>
#include <RH_SX126x.h>

RH_SX126x driver(SS, 7, 8, 9); // NSS, DIO1, BUSY, NRESET

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
  //driver.setModemConfig(RH_SX126x::LoRa_Bw125Cr45Sf2048);
  
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


