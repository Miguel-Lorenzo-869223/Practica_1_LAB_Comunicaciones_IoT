// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 13/09/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

/*
  The Arduino Nano 33 BLE Sense Lite is based on the nRF52840 MCU, which does
  not provide the classic Arduino timers (Timer1, Timer2, etc.). Instead, it
  includes five 32‑bit hardware timers (TIMER0–TIMER4) driven by the 16 MHz
  high‑frequency clock.

  The Arduino/mbed core internally uses TIMER0, TIMER1 and TIMER2 for system
  tasks, PWM generation and the BLE stack. Therefore, only TIMER3 and TIMER4
  are safe for user applications.

  The BBTimer library provides a simple interface to configure these hardware
  timers directly. It allows:
      - Setting precise periods in microseconds.
      - Running a user callback inside the timer interrupt.
      - Starting, stopping and updating the timer period.

  This library Created by B Bryant on 9/15/2021 is used because the Arduino API does 
  not expose the nRF52840 hardware timers. BBTimer enables accurate periodic interrupts 
  without relying on millis() or delay(), making it ideal for tasks such as periodic ADC
  sampling. 
  https://forum.arduino.cc/t/five-hardware-timers-example/905798
*/

#include <Arduino.h>
#include "BBTimer.hpp"
#define ADC_0 A0
#define SAMPLE_FREC 10 // Acquisition frequency in Hz

// Use BB_TIMER3 to avoid conflicts with Mbed OS
BBTimer timer_3_adc(BB_TIMER3);

// Use BB_TIMER3 to avoid conflicts with Mbed OS
volatile bool sample = false;

// LED blink variables
unsigned long leds_prev_time = 0;
const long blink_led_time = 200; // Parpadeo cada 200 ms (rápido)
bool blink_state = false;

//FUNCTIONS DECLARATION
void printer(int adc_level, char* volt_str);

//Timer ISR: runs every 10 secondss
void timerCallback() {
    sample = true;
}

void setup() {
    Serial.begin(115200);

    // Configure onboard LEDs
    pinMode(LED_BUILTIN, OUTPUT);

    #ifdef LED_POWER
    pinMode(LED_POWER, OUTPUT);
    #endif

    // Wait up to 4 seconds for Serial
    uint32_t ticks = millis();
    while (!Serial && (millis() - ticks < 4000));
    Serial.println("OK... RUNNING");

    // Start 10‑second timer (10,000,000 µs)
    timer_3_adc.setupTimer(SAMPLE_FREC*1000000, timerCallback);
    timer_3_adc.timerStart();
}

void loop() {

    unsigned long curr_time = millis();
    if (curr_time - leds_prev_time >= blink_led_time) {
        leds_prev_time = curr_time;
        blink_state = !blink_state;

        digitalWrite(LED_BUILTIN, blink_state ? HIGH : LOW);
        #ifdef LED_POWER
        digitalWrite(LED_POWER, estadoLEDs ? HIGH : LOW);
        #endif
    }

    if (sample) {
        sample = false;
        int adc_level = analogRead(ADC_0);
        float voltage = (adc_level/1024.0)*3.3;
        String volt_str = String(voltage, 3);
        printer(adc_level, volt_str);
    }
}

void printer(int adc_level, String volt_str){
    char buffer[60];
    sprintf(buffer, "ADC=%d  Voltage=%s V", adc_level, volt_str.c_str());
    Serial.println(buffer);
}
