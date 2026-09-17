#pragma once
#include <Arduino.h>

// Initialize servo, RFID, and button
void initDoor();

// Call this in your fast loop. Returns 'true' if the door state just changed.
bool updateDoor();

// Used by Firebase stream to force the door open/closed
void setDoorState(bool open);

// Getter for uploading current status to Firebase
bool getDoorState();