#pragma once
#include <Arduino.h>

// Initialize sensor pins and hardware
void initSensors();

// Read data from sensors
void updateSensors();

// Check if the gas level threshold is reached
bool isGasDangerous();

// Read the temperature from the DHT Sensor
float getTemperature();

// Read the humidity from the DHT Sensor
float getHumidity();