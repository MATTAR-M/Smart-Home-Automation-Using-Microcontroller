#pragma once

// Initial connection for WiFi and Firebase
void initCloud();

// Call in the fast loop to reconnect if WiFi drops
void maintainCloudConnection();

// Check if Firebase is ready to accept data
bool isCloudReady();

// Upload routine for periodic sensor telemetry
void uploadSensorData(float temp, float humidity, bool isGasDangerous);

// Specific sync for door events
void syncDoorStateToCloud(bool isDoorOpen);

// Allows main.cpp to update Firebase when the LDR triggers the lights
void syncOutsideLightsToCloud(bool lightsOn);