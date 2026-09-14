#include <Arduino.h>
#include "mbed.h"

#define PWM_PIN D2
#define PWM_FREC 1000 //Frec in kHz
#define PWM_DUTY 200 //Duty Cycle (%) 0-255

mbed::PwmOut pwm(digitalPinToPinName(PWM_PIN));

void setup() {
  Serial.begin(115200);
  pinMode(PWM_PIN, OUTPUT);
  pwm.period_us(1000000 / PWM_FREC);
}

void loop() {
  pwm.period_us(1000000 / PWM_FREC);
  pwm.write(PWM_DUTY / 255.0f);

  // 3. Imprimir datos por el puerto serie
  Serial.print("Frecuencia: ");
  Serial.print(PWM_FREC);
  Serial.print(" Hz | Duty: ");
  Serial.println(PWM_DUTY);

  delay(100);
}