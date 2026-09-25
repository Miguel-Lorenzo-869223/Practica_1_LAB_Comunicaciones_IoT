// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 17/09/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

#include <Arduino.h>
#include <Wire.h>
#define PACKAGE 3
#define I2C_SLAVE_ADDR 0x08


volatile bool newData = false;

// Tres floats recibidos del maestro
float ax = 0.0f;
float ay = 0.0f;
float az = 0.0f;

void recibe_data(int bytes);

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 4000);
  Wire.begin(I2C_SLAVE_ADDR);
  Wire.onReceive(recibe_data);
  
  Serial.println("--- I2C SLAVE READY ---");
}

void loop() {
  if (newData) {
    Serial.print("AX: "); Serial.println(ax, 3);
    Serial.print("AY: "); Serial.println(ay, 3);
    Serial.print("AZ: "); Serial.println(az, 3);
    Serial.println("-----------------------");

    newData = false;
  }
}

void recibe_data(int bytes) {
  if (bytes == PACKAGE * sizeof(float)) {   // Esperamos 12 bytes
    uint8_t raw[12];

    for (int i = 0; i < 12; i++) {
      raw[i] = Wire.read();
    }

    // Reconstruir los tres floats
    memcpy(&ax, raw, sizeof(float));        // bytes 0..3
    memcpy(&ay, raw + 4, sizeof(float));    // bytes 4..7
    memcpy(&az, raw + 8, sizeof(float));    // bytes 8..11

    newData = true;
  }
}
