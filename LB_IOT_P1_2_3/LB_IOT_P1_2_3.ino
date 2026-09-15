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
#define PWM_FREC 5000 //Frec in Hz
#define SAMPLE_FREC 10 // Acquisition frequency in Hz

mbed::PwmOut pwm(digitalPinToPinName(PWM_PIN));

bool sample = false;
uint32_t ticks = 0, curr_time = 0;
const uint32_t sample_interval = 1000 / SAMPLE_FREC; // 100 ms

//FUNCTIONS DECLARATION
void printer(int adc_level);

void setup() {
  Serial.begin(115200);
  pinMode(ADC_0, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);

  pwm.period_us(1000000 / PWM_FREC);
  digitalWrite(LED_BUILTIN, HIGH);

  ticks = curr_time;
}

void loop() {
  curr_time = millis(); //update time
  if (millis() - ticks >= (1000 / SAMPLE_FREC)) {
      ticks = millis();

      int adc_val = analogRead(ADC_0);
      pwm.write((float)adc_val / 1023.0f);
      printer(adc_val);
    }
}


void printer(int adc_level){
  float duty_cycle = (float)adc_level / 1023.0f;
  
  Serial.print("ADC: ");
  Serial.print(adc_level);
  Serial.print(" | Voltaje: ");
  Serial.print(duty_cycle * 3.3f, 2);
  Serial.print(" V | Duty: ");
  Serial.print(duty_cycle * 100.0f, 1);
  Serial.println("%");
}