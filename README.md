# Introduction

This project is a modular sensor data logger system designed to simulate real-world vehicle telemetry and visualize it in real-time.

It combines microcontrollers, CAN simulation, MQTT messaging, and cloud-style dashboards.

---

## Parts List

| Component                  | Description                                                       |
|----------------------------|-------------------------------------------------------------------|
| STM32 (Blue Pill)          | Reads accelerometer (ADXL345) data over I2C and sends it via UART |
| Raspberry Pi               | Acts as a gateway, reading UART or CAN and publishing to MQTT     |
| CAN HAT + Pi               | Simulates fake CAN messages (speed, RPM) for testing              |
| Node-RED + InfluxDB        | Visualizes real-time data and stores it in a time-series DB       |

---

## Features

- Live gauges for speed and RPM (CAN)
- Accelerometer data logger (STM32 → UART → MQTT)
- MQTT-based communication
- InfluxDB time-series storage
- Web-based dashboard (Node-RED)

---

## Project Structure

Sensor-Data-logger/

├── stm32_firmware/       # Firmware for ADXL345 (I2C) + UART output

├── raspberry_pi/         # UART parsing + MQTT publisher

├── can_logger/           # CAN sender + CAN→MQTT bridge

├── node_red/             # Node-RED flow + dashboard config

├── influxdb/             # InfluxDB notes/scripts

├── docs/                 # screenshots or project documentation

└── README.md             

---

## Flow Diagram
```text
[ ADXL345 ]
     ↓
[ STM32 (reads via I2C) ]
     ↓ UART
[ Raspberry Pi (UART to MQTT) ]
     ↓ MQTT topic: sensor/accel
[ Node-RED Dashboard + InfluxDB ]


Alternatively:

[ CAN Sender (Fake data generator) ]
     ↓ sends via can0
[ CAN-MQTT Bridge ]
     ↓ MQTT topic: sensor/can
[ Node-RED Dashboard + InfluxDB ]

```
---

## Tools & Technologies Used

| Tool             | Purpose                                                             |
|------------------|---------------------------------------------------------------------|
| STM32CubeIDE     | Writing and compiling embedded C firmware for STM32                 |
| STM32 SPL        | Standard Peripheral Library for low-level hardware access           |
| g++              | Compiling C++ programs on the Raspberry Pi (UART/CAN logic)         |
| Mosquitto        | MQTT broker and client for lightweight message transport            |
| Node-RED         | Visual dashboard for real-time monitoring and control               |
| InfluxDB         | Time-series database for storing sensor/CAN data                    |
| CAN-utils        | Tools like 'cansend', 'candump' to test and debug CAN interfaces    |
| Linux (Raspbian) | OS running on the Raspberry Pi                                      |
| MCP2515 CAN HAT  | CAN interface hardware for Raspberry Pi                             |
| ADXL345          | 3-axis accelerometer sensor connected to STM32 via I2C              |

---

## Author

Khaled Hidoussi – 2024
