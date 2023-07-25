/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPL-YkyNps_"
#define BLYNK_DEVICE_NAME "Quickstart Template"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 21
#define RST_PIN 22
int lightPin = 5;
int lightValue;


MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
byte nuidPICC[4] = {0xC2, 0x60, 0xE0, 0x1B};

char auth[] = "I9CwFRWnv8XKOZ8HcqLz9DP8lO2x0NZD";
char ssid[] = "SmS_jiofi";
char pass[] = "sms123458956";

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}



void setup()
{

  Serial.begin(9600);
  pinMode(lightPin,OUTPUT);
  digitalWrite(lightPin,LOW);
  Blynk.begin(auth, ssid, pass);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
}

void loop()

{
  Blynk.run();
  if ( ! rfid.PICC_IsNewCardPresent())
    return;


  if ( ! rfid.PICC_ReadCardSerial())
    return;
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));


  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  if (rfid.uid.uidByte[0] == nuidPICC[0] ||
      rfid.uid.uidByte[1] == nuidPICC[1] ||
      rfid.uid.uidByte[2] == nuidPICC[2] ||
      rfid.uid.uidByte[3] == nuidPICC[3] ) {
    Serial.println(F("ID RECOGNISED"));

    digitalWrite(lightPin, HIGH);
    Blynk.virtualWrite(V0,1);

    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }

    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();

  }

  else {
    Serial.println(F("DO NOT MATCH."));

    digitalWrite(lightPin, LOW);
    Blynk.virtualWrite(V0, lightValue);
  }




  rfid.PICC_HaltA();


  rfid.PCD_StopCrypto1();


}
