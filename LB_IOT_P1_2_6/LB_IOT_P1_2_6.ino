// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 17/09/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

#include <Arduino.h>
#include <Wire.h>
#include <Arduino_LSM9DS1.h>

#define I2C_SLAVE_ADDR 0x08

// Dynamic data variables
float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;

unsigned long pv_millis_sample, pv_millis_print = 0;
const long interval_sample = 200; // Sample and send every 200 ms
const long interval_print = 1000; // Sample and send every 1000 ms

// FUNCTIONS DECLARATION
void read_imu();
void printer();

void setup() {
  Wire.begin(); // Master I2C setup
  Serial.begin(115200);
  while (!Serial && millis() < 4000);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {
  unsigned long current_millis = millis();

  if(current_millis - pv_millis_sample >= interval_sample){
    pv_millis_sample = current_millis;
    read_imu();
  }

  if(current_millis - pv_millis_print >= interval_print){
    pv_millis_print = current_millis;
    printer();
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