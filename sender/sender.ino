#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>

#define RADIO_CS_PIN    18
#define RADIO_RST_PIN   23
#define RADIO_DIO0_PIN  26
#define RADIO_MOSI_PIN  27


int counter = 1;

void setup() {
  Serial.begin(115200);
  Serial.println("LoRa Sender");
  LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(12);
}

void loop() {
  if (Serial.available() > 0) {
    String inputData = Serial.readStringUntil('\n');
    String loraPacket = String(counter) + ": " + inputData;

    Serial.print("Sending packet: ");
    Serial.println(loraPacket); 

    LoRa.beginPacket();
    LoRa.print(loraPacket);
    LoRa.endPacket();

    counter++;
    delay(5000);
  }
}
