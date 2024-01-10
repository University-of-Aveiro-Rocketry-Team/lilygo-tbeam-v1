#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>


#define RADIO_CS_PIN    18
#define RADIO_RST_PIN   23
#define RADIO_DIO0_PIN  26
#define RADIO_MOSI_PIN  27

#define SS_RX 32  // Software Serial RX Pin
#define SS_TX 25  // Software Serial TX Pin
SoftwareSerial mySerial(SS_RX, SS_TX); // RX, TX

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
  LoRa.setSpreadingFactor(7);

  mySerial.begin(9600); // Start software serial
  Serial.println("Software Serial Initialized");
  mySerial.println();
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet '");

    String recv = "";
    while (LoRa.available()) {
      recv += (char) LoRa.read();
    }

    // Extract the counter value from the received packet
    int separatorIndex = recv.indexOf(':');
    if (separatorIndex != -1) {
      mySerial.println(recv);

      String counterString = recv.substring(0, separatorIndex);
      int receivedPacket = counterString.toInt();

      Serial.print(recv); // Print the whole packet
      Serial.print("' with counter: ");
      Serial.println(receivedPacket);

      if (receivedPacket != expectedPacket) {
        Serial.print("Packet loss! Expected: ");
        Serial.print(expectedPacket);
        Serial.print(", Received: ");
        Serial.println(receivedPacket);
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

      expectedPacket = receivedPacket + 1;
      Serial.println();
    }
  }
  delay(1);
}
