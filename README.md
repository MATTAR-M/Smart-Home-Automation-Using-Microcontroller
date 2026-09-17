# Smart Home Automation System (ESP32 & Firebase)

A comprehensive IoT smart home system developed as a Computer Science graduation project. This system provides real-time environmental monitoring, secure RFID door access, automated climate control, and bi-directional cloud synchronization using an ESP32 microcontroller and Firebase Realtime Database. 

The codebase is engineered using backend software development principles, featuring a strictly modular C++ architecture with decoupled domain controllers, hardware abstraction, and isolated cloud services.

## Core Features

* **Secure Access Control:** RC522 RFID and push-button entry paired with a servo-driven door mechanism and debounce-protected inputs.
* **Environmental Safety:** MQ-2 gas leak detection with an immediate localized buzzer alarm and automated kitchen hood activation.
* **Automated Climate & Lighting:** PIR motion-activated security lights, LDR-triggered exterior lighting, and automated cooling fans based on DHT22 temperature thresholds.
* **Real-Time Cloud Synchronization:** Continuous bi-directional telemetry via Firebase RTDB, allowing remote monitoring and manual mobile app overrides.
* **Local UI:** 16x2 I2C LCD for real-time temperature, humidity, and door state feedback.

## Hardware Components

| Component | Function |
| :--- | :--- |
| **ESP32-WROOM-32** | Core microcontroller and Wi-Fi interface |
| **MQ-2 Gas Sensor** | Detects gas leaks and triggers safety protocols |
| **DHT22** | Monitors indoor temperature and humidity |
| **HC-SR501 PIR** | Detects motion for localized security lighting |
| **Photoresistor (LDR)** | Measures ambient light for exterior illumination |
| **RC522 RFID Reader** | Authenticates secure door access via SPI |
| **SG90 Micro Servo** | Actuates the physical door locking mechanism |
| **16x2 I2C LCD** | Displays system telemetry and temporary alerts |
| **4-Channel Relay** | Switches high-voltage loads (fans, kitchen hood, lights) |
| **Active Buzzer** | Emits door access beeps and continuous gas alarms |

## Software Architecture

The project abandons the traditional monolithic Arduino `loop()` in favor of a robust, production-ready embedded C++ structure. 

* **Domain Controllers (`SensorManager`, `DoorController`, `EnvironmentController`):** Encapsulate hardware logic, private variables, and specific sensor thresholds to prevent global state mutation.
* **Cloud Abstraction (`CloudService`):** Isolates the Firebase ESP Client and Wi-Fi reconnection loops from the physical hardware logic.
* **Hardware Unit Testing (`test/`):** Utilizes isolated validation scripts to verify individual I/O modules prior to full system integration.

### Directory Structure

```t
├── include/                 # Header files defining module interfaces
│   ├── CloudService.h
│   ├── configExample.h      # Credential template
│   ├── DoorController.h     
│   ├── EnvironmentController.h
│   ├── pins.h               # Centralized GPIO pin mappings
│   └── SensorManager.h
├── src/                     # C++ logic implementation
│   ├── CloudService.cpp
│   ├── DoorController.cpp
│   ├── EnvironmentController.cpp
│   ├── main.cpp             # Clean orchestrator
│   └── SensorManager.cpp
├── test/                    # Isolated hardware component tests
│   ├── Component_Testing/
│   ├── Sensor_Testing/
│   └── System/
└── docs/                    # Visual assets and schematics
```

Setup & Installation
## 1. Prerequisites
VS Code with the PlatformIO IDE extension installed.

A Firebase project configured with a Realtime Database (Test Mode or with authenticated rules).

## 2. Configuration
Clone the repository and open the folder in VS Code:
```bash
git clone [https://github.com/YOUR-USERNAME/YOUR-REPO-NAME.git](https://github.com/YOUR-USERNAME/YOUR-REPO-NAME.git)
cd YOUR-REPO-NAME
```
Navigate to include/configExample.h and rename it to include/config.h. Update the file with your live network and database credentials:

C++
#pragma once

// --- WiFi Credentials ---
#define WIFI_SSID     "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// --- Firebase Credentials ---
#define DATABASE_URL  "YOUR_FIREBASE_URL"
#define API_KEY       "YOUR_FIREBASE_API_KEY"
#define USER_EMAIL    "YOUR_AUTH_EMAIL"
#define USER_PASSWORD "YOUR_AUTH_PASSWORD"


## 3. Build and Upload
Connect your ESP32 to your computer via USB.

Build the project to automatically download library dependencies:
```bash
pio run
```