#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>


#define RADIO_CS_PIN    18
#define RADIO_RST_PIN   23
#define RADIO_DIO0_PIN  26
#define RADIO_MOSI_PIN  27

#define SS_RX 32  // Software Serial RX Pin
#define SS_TX 33  // Software Serial TX Pin
SoftwareSerial mySerial(SS_RX, SS_TX); // RX, TX


int counter = 1;

void setup() {
  Serial.begin(115200);
  Serial.println("LoRa Sender");
  LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(10);

  mySerial.begin(9600); // Start software serial
  Serial.println("Software Serial Initialized");
}

void loop() {
  static String inputData = "";
  while (mySerial.available()) {
    char incomingChar = (char)mySerial.read();
    
    // Check if the character is printable
    if (isPrintable(incomingChar) || incomingChar == '\n') {
      inputData += incomingChar;
    }

    // Check for the newline character
    if (incomingChar == '\n') {
      String loraPacket = String(counter) + ": " + inputData;

      Serial.print("Sending packet: ");
      Serial.println(loraPacket);

      LoRa.beginPacket();
      LoRa.print(loraPacket);
      LoRa.endPacket();

      counter++;
      inputData = ""; // Reset the string for the next message
    }

    // Prevent inputData from exceeding a safe size
    if (inputData.length() > 128) {
        inputData = ""; // Reset if it gets too long
    }
  }
}
