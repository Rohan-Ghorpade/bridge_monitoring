# bridge_monitoring

# IoT-Based Road Bridge Health Monitoring and Warning System

An IoT-enabled project to monitor the structural health of bridges in real-time using multiple sensors, cloud connectivity, and automation to enhance public safety and infrastructure maintenance.

---

## 🔍 Project Overview

This system uses sensors such as **vibration sensors**, **load cells**, **DHT22 (temperature & humidity)**, **MPU6050 (accelerometer/gyroscope)**, **IR sensors**, **water level sensors**, and **flex sensors** to continuously monitor parameters that affect bridge stability. Data is transmitted via **ESP8266 NodeMCU** to a cloud-based dashboard.

In the event of any detected anomaly (e.g., excessive vibration, tilt, water level rise, or load), a **warning system** activates: buzzer alert, LED indicator, and servo-based barrier control.

---

## 🧰 Hardware Used

- **Arduino UNO R3 WiFi**
- **ESP8266 NodeMCU**
- **MPU6050 Accelerometer/Gyroscope**
- **Vibration Sensor**
- **Load Sensor (5kg)**
- **DHT22 Temperature and Humidity Sensor**
- **Water Level Sensor**
- **Flex Sensor**
- **Infrared Sensor**
- **Servo Motor**
- **1KΩ Resistors, LEDs, Buzzer**

---

## 💻 Software & Tools

- **Arduino IDE**
- **Embedded C (Arduino Language)**
- **ESP8266 WebServer Library**
- **Adafruit MPU6050 & DHT Libraries**
- **HX711 Load Cell Library**

---

## 🛠️ System Architecture

- Sensor Data Collection → Arduino UNO R3
- Data Transmission via UART → ESP8266
- ESP8266 Web Server → Real-time Monitoring Dashboard
- Trigger Alerts → Servo (barrier), Buzzer, LEDs

---

## 🌐 Live Monitoring Interface

ESP8266 hosts a local web page displaying:

- Sensor Readings (Temperature, Humidity, Vibration, Tilt, Water Level, etc.)
- Real-time bridge status (SAFE/UNSAFE)
- Alerts and Reason Logs

**Refresh Rate:** 1 second  
**UI:** Responsive, auto-refreshing with CSS styling

---

## 🧪 Functional Features

- Real-time anomaly detection
- Barrier control (Servo motor)
- Automatic LED lighting using LDR
- Audible alerts via buzzer
- Mobile/PC-based web interface

---

## 📊 Example Dashboard Screenshot

![Alt text](https://github.com/Rohan-Ghorpade/bridge_monitoring/blob/main/Bridge_prototype.png)



![Alt text](https://github.com/Rohan-Ghorpade/bridge_monitoring/blob/67e7092df54d7fb35745e9c96c20d2fe487c38c9/web_page_interface.png)

---

## 📚 References

- [Arduino Official Documentation](https://www.arduino.cc/reference/en/)
- [ESP8266 NodeMCU Docs](https://randomnerdtutorials.com/esp8266-nodemcu-arduino-ide/)
- [Adafruit MPU6050 Guide](https://learn.adafruit.com/using-mpu6050-with-arduino)
- [DHT22 Sensor Details](https://learn.adafruit.com/dht/overview)
- [HX711 Load Cell Amplifier](https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide)
- [IoT-Based Structural Health Monitoring — IEEE Papers](https://ieeexplore.ieee.org/)
- CMR Institute of Technology, Department of ECE – Project Guidance

---

