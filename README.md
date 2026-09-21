# Practica_1_LAB_Comunicaciones_IoT
# Practical Exercise 1: Peripherals, Timers, PWM, Serial Protocols & I2C Bus Interconnection

> **IoT Communications Laboratory**  
> MSc in Electronic Engineering — University of Zaragoza (EINA/UNIZAR)  
> **Author:** Miguel A. Lorenzo  
> **Date:** September 2026  

---

## 📌 Project Overview

This repository contains the full embedded C++ software implementation for **Practical Exercise 1** of the IoT Communications Laboratory. The main objective is to master low-level microcontroller hardware control, including Analog-to-Digital Conversion (ADC), hardware timers and ISR interrupts, PWM signal synthesis, custom UART string protocols, onboard IMU sensor reading, and inter-board I2C master-slave communication.

---

## 🛠️ Hardware Setup & Key Peripherals

* **Master Development Board:** Microcontroller with ADC, PWM, UART, and I2C hardware peripherals.
* **Slave Development Board (Exercises 5 & 7):** **Arduino Nano 33 IoT** connected via I2C bus.
* **Analog Inputs:** Potentiometer supplied via **3V3 output pin** (verified $\le 3.3\text{V}$ threshold).
* **Outputs:** Status LEDs and PWM generation pin ($5\text{ kHz}$).
* **Sensors (Exercise 6 & 7):** Onboard 9-DOF IMU (Accelerometer, Gyroscope, and Magnetometer).

---

## 🏗️ Exercises & System Architectures

1. **Exercise 1.2.1 — Basic ADC Polling:**
   Reads potentiometer voltage using non-blocking time intervals (`millis()`) every 1000 ms and outputs formatted data via UART (`sprintf`).
2. **Exercise 1.2.2 — Hardware Timer Interrupts:**
   Configures an internal hardware timer to trigger a periodic Interrupt Service Routine (ISR) every 10 seconds for ADC sampling and reporting.
3. **Exercise 1.2.3 — ADC-Proportional PWM Synthesis:**
   Generates a high-frequency $5\text{ kHz}$ PWM signal whose duty cycle dynamically scales based on real-time ADC readings.
4. **Exercise 1.2.4 — Custom UART Command Protocol:**
   Processes ASCII strings received via Serial to control hardware modes:
   * `ADC`: Triggers an immediate ADC readout.
   * `ADC(x)`: Configures a hardware timer interrupt to report ADC data every `x` seconds (`x=0` disables transmission).
   * `PWM(x)`: Sets the PWM duty cycle from steps `0` to `9`.
5. **Exercise 1.2.5 — I2C Master-Slave LED Control:**
   Establishes I2C master-slave communication between two boards. The master sends commands to toggle an external digital output LED on the **Arduino Nano 33 IoT** slave.
6. **Exercise 1.2.6 — Onboard IMU Sensor Suite:**
   Concurrently polls 3-axis accelerometer, gyroscope, and magnetometer registers every 100 ms, outputting aggregated telemetry packets over UART every 1000 ms.
7. **Exercise 1.2.7 — Distributed I2C IMU Telemetry:**
   Combines sensor sampling and I2C messaging:
   * **Master Board:** Captures IMU data every 200 ms for a 1-second burst upon user request, then transmits the buffered block over I2C.
   * **Slave Board (Arduino Nano 33 IoT):** Receives the I2C packet, outputs data to Serial, and flashes a status LED for 1 second.

---

## 📂 Project Structure

```text
.
├── LB_IOT_P1_2_1/
│   └── LB_IOT_P1_2_1.ino       # Exercise 1: ADC sampling via millis() & sprintf
├── LB_IOT_P1_2_2/
│   ├── BBTimer.hpp             # Hardware timer library header (BitBang_Timer)
│   └── LB_IOT_P1_2_2.ino       # Exercise 2: Hardware timer interrupt (10s ADC)
├── LB_IOT_P1_2_3/
│   └── LB_IOT_P1_2_3.ino       # Exercise 3: 5kHz PWM output scaled by ADC
├── LB_IOT_P1_2_4/
│   ├── BBTimer.hpp             # Hardware timer library header (BitBang_Timer)
│   └── LB_IOT_P1_2_4.ino       # Exercise 4: Serial String command protocol parser
├── LB_IOT_P1_2_5/
│   ├── LB_IOT_P1_2_5.ino       # Exercise 5: I2C Master LED controller
│   └── LB_IOT_P1_2_5_slave.ino # Exercise 5: I2C Slave (Arduino Nano 33 IoT)
├── LB_IOT_P1_2_6/
│   └── LB_IOT_P1_2_6.ino       # Exercise 6: Onboard 9-DOF IMU telemetry
├── LB_IOT_P1_2_7/
│   ├── LB_IOT_P1_2_7.ino       # Exercise 7: Master IMU burst sampling & I2C TX
│   └── LB_IOT_P1_2_7_slave.ino # Exercise 7: Slave I2C receiver & LED pulse
└── README.md                   # Repository documentation
```

---

## 📚 Reference Material & Documentation

* [sprintf Utility in Arduino](https://www.programmingelectronics.com/sprintf-arduino/)
* [Hardware Timers & Interrupts Setup](https://forum.arduino.cc/t/five-hardware-timers-example/905798)
* [Arduino String Data Type Reference](https://www.arduino.cc/reference/en/language/variables/data-types/string/)
* [Memory Management & String Interning](https://cpp4arduino.com/2018/10/23/what-is-string-interning.html)
* [Arduino Nano 33 BLE Sense / IoT I2C Guide](https://docs.arduino.cc/tutorials/nano-33-ble-sense/i2c/)
* [IMU Accelerometer Tutorial](https://docs.arduino.cc/tutorials/nano-33-ble-sense/imu-accelerometer/)
* [IMU Gyroscope Tutorial](https://docs.arduino.cc/tutorials/nano-33-ble-sense/imu-gyroscope/)
* [IMU Magnetometer Tutorial](https://docs.arduino.cc/tutorials/nano-33-ble-sense/imu-magnetometer/)

---

## 🚀 How to Build & Run

1. Open any of the exercise folders (`LB_IOT_P1_2_X`) in **Arduino IDE**.
2. For dual-board exercises (`1.2.5` and `1.2.7`), upload `.ino` to the Master board and `_slave.ino` to the **Arduino Nano 33 IoT**.
3. Ensure shared ground (`GND`) and I2C line connections (`SDA`, `SCL`) with pull-up resistors where required.
4. Open the Serial Monitor at **115200 baud** to interact with the system or read telemetry data.
