### CAN Integration

In addition to direct sensor logging (ADXL345), this project demonstrates CAN-based data acquisition using a Raspberry Pi and a CAN HAT (MCP2515). This simulates the type of data a real vehicle would output over a CAN bus.

---

### CAN Sender (Simulated Data Generator)

- Sends fake speed and RPM data over the `can0` interface every second.
- Used to simulate in-vehicle telemetry without a real car.

Code: 'can_sender.cpp'  
Example Data Sent:

CAN ID: 0x123
Data[0]: Speed (km/h)
Data[1]: RPM / 100


---

### CAN Receiver + MQTT Bridge

- Reads incoming CAN frames from 'can0'
- Converts them into a JSON message:

{ "speed": 10, "rpm": 2000 }

- Publishes to the MQTT topic: 'sensor/can'

Code: 'can_mqtt_bridge.cpp'

---

### Node-RED Integration

A separate Node-RED flow subscribes to 'sensor/can' and:

- Displays gauges for live speed and RPM
- Saves time-series data to InfluxDB
- Allows visualization over time via charts

---

### Example Node-RED Flow

- MQTT In Node → Topic: 'sensor/can'
- JSON Parser
- Two Gauges (Speed, RPM)
- InfluxDB Out Node → DB: 'sensordata', Measurement: 'can_signals'

---

### CAN Project Structure
```text

can_logger/
├── can_sender.cpp        # Sends fake CAN messages
├── can_mqtt_bridge.cpp   # Reads CAN, publishes to MQTT
├── README.md             # This file


> This CAN module is kept independent of the I2C sensor code to reflect a modular, real-world embedded design architecture.
```
---

## Author

Khaled Hidoussi - 2024

