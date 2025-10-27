#include <Wire.h>
#include <Adafruit_BMP3XX.h>

Adafruit_BMP3XX bmp;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!bmp.begin_I2C()) {  // Automatically uses the default SDA/SCL
    Serial.println("Could not find a valid BMP390 sensor, check wiring!");
    while (1);
  }

  Serial.println("BMP390 Initialized!");
}

void loop() {
  if (!bmp.performReading()) {
    Serial.println("Failed to perform reading!");
    return;
  }

  Serial.print("Temperature = ");
  Serial.print(bmp.temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bmp.pressure / 100.0);
  Serial.println(" hPa");

  delay(1000);
}
