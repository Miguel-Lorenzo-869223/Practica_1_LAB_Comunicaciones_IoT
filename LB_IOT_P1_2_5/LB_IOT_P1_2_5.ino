#include <Arduino.h>
#include <Wire.h>

#define I2C_SLAVE_ADDR 8
#define LED_ON 1
#define LED_OFF 0

void send_i2c_command(uint8_t cmd);

void setup() {
  Wire.begin(); // Inicializa I2C como Maestro
  Serial.begin(115200);
  while (!Serial && millis() < 4000);

  Serial.println("--- MAESTRO I2C LISTO ---");
  Serial.println("Enviando comandos '1' y '0' cada segundo...");
}

void loop() {
  // Encender LED en la esclava
  send_i2c_command(LED_ON);
  Serial.println("I2C -> LED ON");
  delay(1000);

  // Apagar LED en la esclava
  send_i2c_command(LED_OFF);
  Serial.println("I2C -> LED OFF");
  delay(1000);
}

void send_i2c_command(uint8_t cmd) {
  Wire.beginTransmission(I2C_SLAVE_ADDR);
  Wire.write(cmd);
  Wire.endTransmission();
}