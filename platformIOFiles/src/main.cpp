#include <Wire.h>
#include <Adafruit_BMP3XX.h>
#include <SPI.h>
#include <Ethernet_Generic.h>
#include <EthernetUdp.h>
#include <DHT.h>

Adafruit_BMP3XX bmp;

#define DHTPIN 2
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);        // Arduino static IP
IPAddress destIP(192, 168, 1, 2);    // PC/server IP
unsigned int destPort = 5005;          // UDP destination port

EthernetUDP Udp;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!bmp.begin_I2C()) {  // Automatically uses the default SDA/SCL
    Serial.println("Could not find a valid BMP390 sensor, check wiring!");
    while (1);
  }

  Serial.println("BMP390 Initialized!");

  Ethernet.begin(mac, ip);
  dht.begin();
  Udp.begin(8888); // local UDP port (can be anything)
  Serial.begin(115200);
  Serial.print("UDP sender ready at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  if (!bmp.performReading()) {
    Serial.println("Failed to perform reading!");
    return;
  }

  float bmpT = bmp.temperature;

  float bmpP = bmp.pressure / 100.0;

  float dhtH = dht.readHumidity();
  float dhtT = dht.readTemperature();

  if (isnan(dhtH) || isnan(dhtT) || isnan(bmpT) || isnan(bmpP)) return; // skip bad readings

  // --- Send both floats as binary (8 bytes total)
  Udp.beginPacket(destIP, destPort);
  Udp.write((byte*)&dhtT, sizeof(float)); // 4 bytes temperature
  Udp.write((byte*)&dhtH, sizeof(float)); // 4 bytes humidity
  Udp.write((byte*)&bmpT, sizeof(float)); // 4 bytes BMP temperature
  Udp.write((byte*)&bmpP, sizeof(float)); // 4 bytes BMP pressure
  Udp.endPacket();

  delay(50); // 20 readings per second

}
