#pragma once
#include <Arduino.h>

// --- Initialization ---
void initEnvironment();

// --- Main Loops ---
// Run this in the fast loop (handles motion, LDR, and quick buzzer beeps)
void updateEnvironmentFast(bool isGasDangerous);

// Run this in the slow loop (handles fans, hood, and LCD refreshes)
void updateEnvironmentSlow(float temp, float humidity, bool isGasDangerous);

// --- Cloud/App Overrides (Setters) ---
void setAutoMode(bool mode);
void setAppFan(bool state);
void setAppLights(bool state);
void setAppHood(bool state);

// --- Hardware Triggers ---
// Call this from main.cpp when the door opens to trigger a short beep
void triggerDoorBeep();

// Call this to override the LCD temporarily (e.g., "Door is open")
void showTemporaryMessage(String message);

// --- Sync Flags ---
// Returns true if auto-mode changed the outside lights and the cloud needs to be updated
bool shouldSyncLightsToCloud();
bool getAppLightsState();