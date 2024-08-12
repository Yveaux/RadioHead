// sx1262_client.ino
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_SX126x class and a basic SX1262 module connected to an Arduino compatible processor
// It is designed to work with the examples sx1262_server and sx1262_server.
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

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on (LoRa_Bw125Cr45Sf128)
  // You can change the frequency:
  //driver.setFrequency(868.0);

  // You can change the modulation parameters with eg
  //driver.setModemConfig(RH_SX126x::LoRa_Bw125Cr45Sf2048);
  
  // You can change the power level in dBm
}

void loop()
{
  Serial.println("Sending to sx126x_server");
  // Send a message to stm32wlx_server
  uint8_t data[] = "Hello World!";
  
  driver.send(data, sizeof(data));
  
  driver.waitPacketSent(3000);


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
    Serial.println("No reply, is sx126x_server running?");
  }

  delay(1000);
}