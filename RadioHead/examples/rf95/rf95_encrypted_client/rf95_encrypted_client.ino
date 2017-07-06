/* Loca Simple Hello World Client with encrypted communications 
 *  Philippe.Rochat'at'gmail.com
 *  06.07.2017
 */
#include <RH_RF95.h>
#include <RHEncryptedDriver.h>
#include <XXTEA.h>
#include <String.h>

RH_RF95 rf95;     // Instanciate a LoRa driver
XXTEA myCipher;   // Instanciate an XXTEA block ciphering
RHEncryptedDriver myDriver(rf95, myCipher); // Instantiate the driver with those two

float frequency = 868.0; // Change the frequency here. 
unsigned char encryptkey[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; // The very secret key
char HWMessage[] = "Hello World ! I'm happy if you can read me";
uint8_t HWMessageLen;

void setup() {
  HWMessageLen = strlen(HWMessage);
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  Serial.println("LoRa Simple_Encrypted Client");
  if (!rf95.init())
    Serial.println("LoRa init failed");
  // Setup ISM frequency
  rf95.setFrequency(frequency);
  // Setup Power,dBm
  rf95.setTxPower(13);
  myCipher.setKey(encryptkey, 16);
  delay(4000);
  Serial.println("Setup completed");
}

void loop() {
  uint8_t data[myCipher.blockSize()] = {0};
  for(uint8_t i = 0; i< HWMessageLen; i++) data[i] = (uint8_t)HWMessage[i];
  Serial.print("Sending: "); Serial.println((char *)&data);
  myDriver.send(data, sizeof(data)); // Send out ID + Sensor data to LoRa gateway
  delay(4000);
}
