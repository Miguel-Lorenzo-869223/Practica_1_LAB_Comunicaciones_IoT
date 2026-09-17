// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 17/09/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

#include <Arduino.h>
#include <Wire.h>

#define I2C_SLAVE_ADDR 8
#define LED_PIN 2
void receiveEvent(int bytes);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Wire.begin(I2C_SLAVE_ADDR); // Escucha en la dirección 8
  Wire.onReceive(receiveEvent);
}

void loop() {
  // Bucle libre, la recepción va por interrupción
}

void receiveEvent(int bytes) {
  while (Wire.available() > 0) {
    int cmd = Wire.read(); // Lee el byte enviado (1 o 0)
    
    if (cmd == 1) {
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(LED_PIN, HIGH);
    } else if (cmd == 0) {
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(LED_PIN, LOW);

    }
  }
}