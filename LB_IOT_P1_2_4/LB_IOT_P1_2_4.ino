// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 17/09/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

#include <Arduino.h>
#include <string.h>
#include "BBTimer.hpp"
#include "mbed.h"

#define BUFFER_LNG 32
#define ADC_0 A0
#define PWM_PIN D2
#define PWM_FREQUENCY 5000

bool finish_write = false;
int index_bff = 0; 
char buffer_msg[BUFFER_LNG];
int x = 0; 

// Hardware Timer and PWM
BBTimer timer_3_adc(BB_TIMER3);
mbed::PwmOut pwmPin(digitalPinToPinName(PWM_PIN));

volatile bool sample = false;

// Timer ISR: runs every period_seconds
void timerCallback() {
    sample = true;
}

// Global control variables
int period_seconds = 0; // 'x' parameter for ADC(x)
int pwmLevel = 0;       // 'x' parameter for PWM(x) [0..9]

// FUNCTIONS DECLARATION
void read_serial();
void parse_command(char* buffer);

void setup() {
  Serial.begin(115200);
  while(!Serial && millis() < 4000);
  pwmPin.period(1.0 / PWM_FREQUENCY);
}

void loop() {
  read_serial();
  
  if (finish_write) {
      parse_command(buffer_msg);
      
      // Reset buffer for the next reading
      index_bff = 0;
      finish_write = false;
  }

  // Periodic ADC reading triggered by hardware timer interrupt
  if (sample) {
      Serial.println(analogRead(ADC_0));
      sample = false;
  }
}

void read_serial(){
  while(Serial.available() > 0 && !finish_write){
    char c = Serial.read(); 
    if(c == '\n' || c == '\r'){
      if(index_bff > 0){
        buffer_msg[index_bff] = '\0';
        finish_write = true; 
      }
    } else if(index_bff < BUFFER_LNG - 1){
      buffer_msg[index_bff] = c;
      index_bff++;
    }
  }
}

void parse_command(char* buffer) {
  // Case 1: "ADC" -> Single reading
  if (strcmp(buffer, "ADC") == 0) {
    pwmPin.write(0/9.0f);
    timer_3_adc.timerStop();
    Serial.println(analogRead(ADC_0));
  } 
  
  // Case 2: "ADC(x)" -> Periodic reading or Stop (x=0)
  else if (sscanf(buffer, "ADC(%d)", &x) == 1) {
    pwmPin.write(0/9.0f);
    if (x > 0) {
      period_seconds = x;
      timer_3_adc.timerStop();
      timer_3_adc.setupTimer(period_seconds * 1000000UL, timerCallback);
      timer_3_adc.timerStart();
    } else {
      timer_3_adc.timerStop();
    }
  } 
  
  // Case 3: "PWM(x)" -> Output duty cycle adjustment (0 to 9)
  else if (sscanf(buffer, "PWM(%d)", &x) == 1) {
    timer_3_adc.timerStop();
    if ((x >= 0) && (x <= 9)) {
      pwmLevel = x;
      pwmPin.write(pwmLevel / 9.0f);
    } else {
      Serial.println("Error: PWM value must be between 0 and 9.");
    }
  } 
  
  // Invalid command
  else {
    Serial.println("Unrecognized command. Valid formats: ADC, ADC(x), PWM(x)");
  }
}