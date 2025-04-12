BLE-Enabled IoT Step Counter with ESP32 and LSM6DSO Sensor

This project demonstrates a two-part Bluetooth Low Energy (BLE) IoT system built with an ESP32 TTGO LoRa32 and an LSM6DSO accelerometer, focusing on step detection, mobile interaction, and wireless control. It combines hardware interfacing, real-time data processing, and BLE communication into a cohesive, mobile-connected application.

In Part A, the ESP32 is configured as a BLE server exposing a custom service and characteristic. A smartphone client (using the nRF Connect app) can write commands such as "ON" or "OFF" to control an external LED connected to the ESP32’s GPIO. This setup demonstrates a lightweight client-server model over BLE, including bidirectional communication using standard BLE properties like WRITE and READ, with immediate feedback on hardware output.
Demo: https://youtu.be/q1QhIQNmUUA?feature=shared
In Part B, the ESP32 collects real-time acceleration data via I²C (SDA: GPIO21, SCL: GPIO22) from the LSM6DSO sensor. A threshold-based algorithm detects individual steps by identifying peaks in the Z-axis acceleration above 1.2 g, with debounce control to ensure accurate counting. Each step is displayed in the serial monitor and transmitted live to the mobile device via BLE notifications. The ESP32 advertises as "SDSUCS" and automatically updates a characteristic with the current step count, which the mobile app can subscribe to for real-time monitoring.
Demo: https://youtu.be/XaWFeA8Uup4?feature=shared
