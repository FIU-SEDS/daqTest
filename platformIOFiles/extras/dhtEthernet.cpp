#include <SPI.h>
#include <Ethernet_Generic.h>
#include <EthernetUdp.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11   // You can change to DHT11 if needed
DHT dht(DHTPIN, DHTTYPE);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);        // Arduino static IP
IPAddress destIP(192, 168, 1, 2);    // PC/server IP
unsigned int destPort = 5005;          // UDP destination port

EthernetUDP Udp;

void setup() {
  Ethernet.begin(mac, ip);
  dht.begin();
  Udp.begin(8888); // local UDP port (can be anything)
  Serial.begin(115200);
  Serial.print("UDP sender ready at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) return; // skip bad readings

  // --- Send both floats as binary (8 bytes total)
  Udp.beginPacket(destIP, destPort);
  Udp.write((byte*)&t, sizeof(float)); // 4 bytes temperature
  Udp.write((byte*)&h, sizeof(float)); // 4 bytes humidity
  Udp.endPacket();

  delay(50); // 20 readings per second
}
