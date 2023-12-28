#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>

#define RADIO_CS_PIN    18
#define RADIO_RST_PIN   23
#define RADIO_DIO0_PIN  26
#define RADIO_MOSI_PIN  27


int expectedPacket = 1;
int lostPackets = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("LoRa Receiver");

  LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(12);

  synchronizeWithSender();
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet '");

    String recv = "";
    while (LoRa.available()) {
      recv += (char) LoRa.read();
    }

    Serial.println(recv);
    int receivedPacket = recv.toInt();

    if (receivedPacket != expectedPacket) {
      Serial.print("Packet loss! Expected: ");
      Serial.print(expectedPacket);
      Serial.print(", Received: ");
      Serial.println(receivedPacket);
      expectedPacket++;
      lostPackets++;
    }
    
    Serial.print("with RSSI ");
    Serial.println(LoRa.packetRssi());
    
    Serial.print("with SNR ");
    Serial.println(LoRa.packetSnr());
    
    Serial.print("Frequency Error: ");
    Serial.println(LoRa.packetFrequencyError());

    Serial.print("Packets lost: ");
    Serial.println(lostPackets);

    expectedPacket++;
    Serial.println();
  }
}

void synchronizeWithSender() {
  Serial.println("Waiting for synchronization message from sender...");

  while (true) {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      String recv = "";
      while (LoRa.available()) {
        recv += (char) LoRa.read();
      }

      if (recv.equals("Sync")) {
        Serial.println("Synchronization complete. Starting operation.");
        Serial.println("Sending synchronization message to sender...");
        LoRa.beginPacket();
        LoRa.print("Ack");
        LoRa.endPacket();
        return;
      }
    }
  }
}
