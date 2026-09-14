// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 14/09/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

#include <Arduino.h>
#include "mbed.h"

#define PWM_PIN D2
#define ADC_0 A0
#define PWM_FREC 40 //Frec in Hz
#define PWM_DUTY 127 // Fixed 50% duty cycle (127/255)
#define NUM_SAMPLES 50


mbed::PwmOut pwm(digitalPinToPinName(PWM_PIN));

void setup() {
  Serial.begin(115200);
  pinMode(ADC_0, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);

  pwm.period_us(1000000 / PWM_FREC);
  pwm.write(PWM_DUTY / 255.0f);
  digitalWrite(LED_BUILTIN, HIGH);


}

void loop() {

  long sumADC = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {// ADC averaging for data stability
    sumADC += analogRead(ADC_0);
  }
  int adcValue = sumADC / NUM_SAMPLES;

  long targetFrequency = map(adcValue, 0, 1023, 40, 5000);

  pwm.period_us(1000000 / targetFrequency);
  pwm.write(PWM_DUTY / 255.0f);

  float voltage = (adcValue * 3.3f) / 1023.0f;

  Serial.print("ADC (A0): ");
  Serial.print(adcValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 2);
  Serial.print(" V | Frequency: ");
  Serial.print(targetFrequency);
  Serial.println(" Hz");

  delay(100); // Small sampling delay
}