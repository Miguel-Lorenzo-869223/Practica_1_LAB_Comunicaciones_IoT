// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 13/09/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

/* ============================================================
   ADC Read Example – Arduino Nano 33 BLE Sense Lite

   Reads the analog input every second and prints the result.
   Serial.print() outputs values directly and is ideal for Serial Plotter. 
   sprintf() builds a formatted string first,
   useful for structured console messages. Note that some
   Arduino platforms have limited float support in sprintf().

   Ensure the potentiometer output does not exceed 3.3V.
   ============================================================ 
*/
   
#include <Arduino.h>
#define ADC_0 A0
#define SAMPLE_FREC 1 // Acquisition frequency in Hz

bool sample = false;
uint32_t ticks = 0, curr_time = 0;

//FUNCTIONS DECLARATION
void printer(int adc_level, char* volt_str);

void setup() {
  Serial.begin(115200);
  pinMode(ADC_0, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  curr_time = millis(); //start reference
}

void loop() {
  curr_time = millis(); //update time

  sample = (curr_time-ticks>=(1000/SAMPLE_FREC)) ? true : false; 

  if(sample){
    int adc_level = analogRead(ADC_0);
    float voltage = (adc_level/1024.0)*3.3;
    String volt_str = String(voltage, 3);
    printer(adc_level, volt_str);
    ticks = curr_time;
  }
  
}

void printer(int adc_level, String volt_str){
    char buffer[60];
    sprintf(buffer, "ADC=%d  Voltage=%s V", adc_level, volt_str.c_str());
    Serial.println(buffer);
}