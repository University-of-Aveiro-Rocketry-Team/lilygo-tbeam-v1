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
char inputData[512]; // Increase buffer size if needed
int inputDataIndex = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("LoRa Sender");
  LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(9);

  mySerial.begin(9600); // Start software serial
  Serial.println("Software Serial Initialized");
}
/*
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
    if (inputData.length() > 256) {
        inputData = ""; // Reset if it gets too long
    }
  }
}
*/

void loop() {
  while (mySerial.available()) {
    char incomingChar = (char)mySerial.read();
    //Serial.print(incomingChar); // Debug: Print each character received

    if (incomingChar == '\n' || inputDataIndex >= sizeof(inputData) - 1) {
      inputData[inputDataIndex] = '\0'; // Null-terminate the string
      //Serial.print("Complete message: "); // Debug
      //Serial.println(inputData);

      String loraPacket = String(counter) + ": " + inputData;
      Serial.print("Sending packet: ");
      Serial.println(loraPacket);

      LoRa.beginPacket();
      LoRa.print(loraPacket);
      LoRa.endPacket();

      counter++;
      inputDataIndex = 0; // Reset index for next message
    } else {
      inputData[inputDataIndex++] = incomingChar;
    }

    // If the buffer is getting full without receiving a newline, clear it
    if (inputDataIndex >= sizeof(inputData) - 10) {
      Serial.println("Buffer almost full, clearing buffer."); // Debug
      inputDataIndex = 0;
    }
  }
}
