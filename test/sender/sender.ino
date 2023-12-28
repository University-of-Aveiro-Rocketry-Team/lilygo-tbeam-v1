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

  synchronizeWithReceiver();
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  LoRa.beginPacket();
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
  delay(5000);
}

void synchronizeWithReceiver() {
  Serial.println("Sending synchronization message to receiver...");
  LoRa.beginPacket();
  LoRa.print("Sync");
  LoRa.endPacket();

  Serial.println("Synchronization message sent. Waiting for receiver to acknowledge...");

  while (true) {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      String recv = "";
      while (LoRa.available()) {
        recv += (char) LoRa.read();
      }

      if (recv.equals("Ack")) {
        Serial.println("Receiver acknowledged synchronization. Starting operation.");
        return;
      }
    }
  }
}
