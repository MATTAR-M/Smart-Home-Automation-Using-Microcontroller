#include <SPI.h>
#include <MFRC522.h>

// pin setup
#define SS_PIN  27   // (SDA) Green reading wire
#define RST_PIN 32  // (RST) Purple Reset Wire

// RFID Controller Object
MFRC522 rfid(SS_PIN, RST_PIN); 

void setup() {
  Serial.begin(115200); 
  while (!Serial);      

  SPI.begin();          
  rfid.PCD_Init();      
  
  Serial.println("RFID Reader is Ready!");
  Serial.println("Please scan a card or tag...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }
//print the card or tag UID
  Serial.print("Card UID:");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  rfid.PICC_HaltA();
}