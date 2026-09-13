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

// Example using BBTimer class to run five hardware timers on Nano33BLE.  
// See BBTimer.hpp for usage details. 
// Include BBTimer.hpp header within project folder. 

// Construct five timers.  Can construct a global scope, or within setup(), or static in loop().
BBTimer my_t0(BB_TIMER0);
BBTimer my_t1(BB_TIMER1);
BBTimer my_t2(BB_TIMER2);
BBTimer my_t3(BB_TIMER3);
BBTimer my_t4(BB_TIMER4);

// global logicals for example synchronization between timer callback functions and loop().
bool red_on = false;
bool green_on = false;
bool blue_on = false;

// One callback for each timer.  
void t0Callback()
{
	static bool toggle = true;
	digitalWrite(LED_BUILTIN, toggle ? HIGH : LOW);
	toggle = !toggle;

	// example changing period from within callback
	static uint32_t period = 10000;
	period += 10000;
	if (period > 5e5) period = 10000;
	my_t0.updatePeriod(period);
}

void t1Callback()
{
	static bool toggle = true;

	digitalWrite(LED_POWER, toggle ? HIGH : LOW);
	toggle = !toggle;
}

void t2Callback()
{
	red_on = !red_on;
}

void t3Callback()
{
	green_on = !green_on;
}

void t4Callback()
{
	blue_on = !blue_on;
}


void setup() {

	pinMode(LED_BUILTIN, OUTPUT);

	my_t0.setupTimer(500000, t0Callback);
	my_t0.timerStart();

	my_t1.setupTimer(450000, t1Callback);
	my_t1.timerStart();

	my_t2.setupTimer(420000, t2Callback);
	my_t2.timerStart();

	my_t3.setupTimer(390000, t3Callback);
	my_t3.timerStart();

	my_t4.setupTimer(340000, t4Callback);
	my_t4.timerStart();
}

void loop() {

	// these don't like to be set from inside a callback.
	digitalWrite(LED_RED, red_on ? LOW : HIGH);
	digitalWrite(LED_GREEN, green_on ? LOW : HIGH);
	digitalWrite(LED_BLUE, blue_on ? LOW : HIGH);
}
