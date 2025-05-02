InfluxDB Configuration for Sensor Data Logger

This file documents how sensor data is stored in InfluxDB via Node-RED.

Databases Used:

- sensordata: used for accelerometer data from STM32, received on MQTT topic "sensor/accel"
- logger: used for CAN simulation data, received on MQTT topic "sensor/can"

Measurements and Topics:

Topic: sensor/accel
Measurement: acceleration
Fields: x, y, z

Topic: sensor/can
Measurement: can_data
Fields: speed, rpm

Node-RED listens to these MQTT topics, parses the incoming JSON payloads, and writes the data to the corresponding measurements in InfluxDB.

Node-RED to InfluxDB Setup:

Host: localhost
Port: 8086
Protocol: http
Authentication: not required (local setup)

InfluxDB output nodes in Node-RED:
- For topic sensor/accel:
  - Database: sensordata
  - Measurement: acceleration

- For topic sensor/can:
  - Database: logger
  - Measurement: can_data

Example of data stored in 'acceleration':

{
  "x": 12.3,
  "y": -4.5,
  "z": 0.8
}
