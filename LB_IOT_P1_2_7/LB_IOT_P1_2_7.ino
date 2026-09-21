// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 18/09/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------


#include <Arduino.h>
#include <string.h>
#include "BBTimer.hpp"
#include "mbed.h"
#include <Wire.h>
#include <Arduino_LSM9DS1.h>

#define I2C_SLAVE_ADDR 0x08
#define BUFFER_LNG 32


// Dynamic data variables
float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;

bool finish_write = false;
int index_bff = 0; 
char buffer_msg[BUFFER_LNG];
int x = 0; 

// Hardware Timer and PWM
BBTimer timer_3_adc(BB_TIMER3);

volatile bool sample = false;


unsigned long pv_millis_sample = 0;
const long interval_sample = 200; // Sample and send every 200 ms

// FUNCTIONS DECLARATION
void read_imu();
void printer();

// Timer ISR: runs every period_seconds
void timerCallback() {
    sample = true;
}

// Global control variables
int period_seconds = 1; // 'x' parameter for send mesagge.

// FUNCTIONS DECLARATION
void read_serial();
void parse_command(char* buffer);

void setup() {
  Wire.begin(); // Master I2C setup
  Serial.begin(115200);
  while(!Serial && millis() < 4000);
  strcpy(buffer_msg, "ST(1)");
  finish_write = true;

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {
  unsigned long current_millis = millis();

  read_serial();

  if(current_millis - pv_millis_sample >= interval_sample){
    pv_millis_sample = current_millis;
    read_imu();
  }

  if (finish_write) {
      parse_command(buffer_msg);
      index_bff = 0;
      finish_write = false;
  }

  if (sample) {
    printer();
    Wire.beginTransmission(I2C_SLAVE_ADDR);
    Wire.write((uint8_t*)&ax, sizeof(ax)); // Envía los 4 bytes del float
    Wire.endTransmission();
    sample=false;
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
  if (sscanf(buffer, "ST(%d)", &x) == 1) {
    if (x > 0) {
      period_seconds = x;
      timer_3_adc.timerStop();
      timer_3_adc.setupTimer(period_seconds * 1000000UL, timerCallback);
      timer_3_adc.timerStart();
    } else {
      timer_3_adc.timerStop();
    }
  }   
  // Invalid command
  else {
    Serial.println("Unrecognized command. Valid formats: ST(X)");
  }
}

void read_imu(){
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
  }
  // 2. Read Gyroscope (dps)
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);
  }
  // 3. Read Magnetometer (uT)
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(mx, my, mz);
  }
}

void printer(){
  Serial.print("ACC [g]: "); Serial.print(ax); Serial.print(", "); Serial.print(ay); Serial.print(", "); Serial.println(az);
  Serial.print("GYR [dps]: "); Serial.print(gx); Serial.print(", "); Serial.print(gy); Serial.print(", "); Serial.println(gz);
  Serial.print("MAG [uT]: "); Serial.print(mx); Serial.print(", "); Serial.print(my); Serial.print(", "); Serial.println(mz);
  Serial.println("-------------------------------------");
}
