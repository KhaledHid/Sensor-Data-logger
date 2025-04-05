# Raspberry Pi - UART to MQTT Bridge

This folder contains the C++ code used on the Raspberry Pi to read accelerometer data from the STM32 via UART and publish it to an MQTT topic for visualization.

---

## Overview

The STM32 sends accelerometer data in plain text over UART. This C++ program listens to the serial port, parses the data, and publishes it as JSON to the `sensor/accel` MQTT topic.

---

## Features

- Opens '/dev/serial0' (UART port)
- Parses lines like:
  
  X: 123
  Y: -45
  Z: 87
  
- Randomizes 'Z' values to simulate 3-axis data
- Publishes structured JSON using 'mosquitto_pub'

---

## Output Format

json:
{ "x": 123, "y": -45, "z": 87 }


Published via:
bash
mosquitto_pub -t sensor/accel -m '<json>'

---

## How to Compile

bash
g++ -o uart_reader read_uart.cpp


Ensure 'read_uart.cpp' contains the 'readUART()' function and a 'main()' if needed.

---

## How to Run

bash
sudo ./uart_reader


> Requires 'mosquitto-clients' to be installed on the Pi:
bash
sudo apt install mosquitto mosquitto-clients

---

## Hardware Connection

- STM32 TX → Pi RX (GPIO14, physical pin 8)
- STM32 GND → Pi GND

Make sure UART is enabled on the Pi:
bash
sudo raspi-config  # Interface Options → Serial → disable login shell, enable UART


---

## Project Files

```
raspberry_pi/
├── read_uart.cpp
├── read_uart.h
└── README.md 
```
---

Khaled Hidoussi – 2024
