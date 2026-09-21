// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 17/09/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

#include <Arduino.h>
#include <Wire.h>

#define I2C_SLAVE_ADDR 0x08

volatile bool newData = false;
float imuValue = 0.0f;   // Valor recibido del maestro

void receiveEvent(int bytes);

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 4000);

  Wire.begin(I2C_SLAVE_ADDR);
  Wire.onReceive(receiveEvent);

  Serial.println("--- I2C SLAVE READY ---");
}

void loop() {
  if (newData) {
    Serial.print("Received float from Master IMU: ");
    Serial.println(imuValue, 3);   // Mostrar con precisión
    newData = false;
  }
}

void receiveEvent(int bytes) {
  if (bytes == sizeof(float)) {
    uint8_t raw[4];

    for (int i = 0; i < 4; i++) {
      raw[i] = Wire.read();
    }

    // Copiar bytes al float
    memcpy(&imuValue, raw, sizeof(float));

    newData = true;
  }
}