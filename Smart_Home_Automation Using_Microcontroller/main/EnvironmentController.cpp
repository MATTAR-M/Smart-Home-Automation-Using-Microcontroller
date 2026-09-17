#include "EnvironmentController.h"
#include "pins.h"
#include <LiquidCrystal_I2C.h>

// --- Private Hardware Objects ---
static LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Private App State ---
static bool autoMode = true;
static bool appFan = false;
static bool appLights = false;
static bool appHood = false;

// --- Timing & Flags ---
constexpr unsigned long MOTION_TIMEOUT = 10000;
constexpr unsigned long DATA_SHOWING_DURATION = 2000;
constexpr unsigned long DOOR_BEEP_DURATION = 300; // 300ms beep for door

static unsigned long lastMotionTime = 0;
static unsigned long dataShowingTime = 0;
static unsigned long beepStartTime = 0;

static bool motionLightState = false;
static bool isBeeping = false;
static bool syncOutsideLightsFlag = false;

// =============================
// Initialization
// =============================
void initEnvironment() {
    pinMode(PIR_PIN, INPUT);
    pinMode(pirLED_PIN, OUTPUT);
    pinMode(LDR_PIN, INPUT);
    pinMode(outSideLight_PIN, OUTPUT);
    pinMode(MoreOutSideLight_PIN, OUTPUT);
    pinMode(FAN_PIN, OUTPUT);
    pinMode(KH_PIN, OUTPUT);
    pinMode(Buzzer_PIN, OUTPUT);

    lcd.init();
    lcd.backlight();
    lcd.print("System Active");
}

// =============================
// Fast Loop (Motion, Lights, Buzzer)
// =============================
void updateEnvironmentFast(bool isGasDangerous) {
    // 1. Check Motion
    int motion = digitalRead(PIR_PIN);
    if (motion == HIGH) {
        lastMotionTime = millis();
        motionLightState = true;
    }
    if (millis() - lastMotionTime > MOTION_TIMEOUT) {
        motionLightState = false;
    }
    digitalWrite(pirLED_PIN, motionLightState);

    // 2. Check Outside Lights (LDR)
    int ldr = 4095 - analogRead(LDR_PIN);
    bool lightsOn;

    if (autoMode) {
        lightsOn = (ldr < LDR_THRESHOLD);
        if (lightsOn != appLights) {
            appLights = lightsOn;
            syncOutsideLightsFlag = true; // Tell main.cpp to update Firebase
        }
    } else {
        lightsOn = appLights;
    }
    digitalWrite(outSideLight_PIN, lightsOn);
    digitalWrite(MoreOutSideLight_PIN, lightsOn);

    // 3. Resolve Buzzer Conflict (Gas overrides Door Beep)
    if (isGasDangerous) {
        digitalWrite(Buzzer_PIN, HIGH);
    } else if (isBeeping) {
        if (millis() - beepStartTime > DOOR_BEEP_DURATION) {
            isBeeping = false;
            digitalWrite(Buzzer_PIN, LOW); // Turn off after 300ms
        } else {
            digitalWrite(Buzzer_PIN, HIGH);
        }
    } else {
        digitalWrite(Buzzer_PIN, LOW);
    }
}

// =============================
// Slow Loop (Fans, Hood, LCD)
// =============================
void updateEnvironmentSlow(float temp, float humidity, bool isGasDangerous) {
    // 1. Control Fan
    bool fanOn = autoMode ? (temp >= FAN_TEMP_THRESHOLD) : appFan;
    digitalWrite(FAN_PIN, fanOn);

    // 2. Control Kitchen Hood
    bool hoodOn = autoMode ? isGasDangerous : appHood;
    digitalWrite(KH_PIN, hoodOn);

    // 3. Update LCD
    if (isGasDangerous) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Gas leak Detected!");
        return;
    }

    // Do not overwrite temporary messages (like "Door is open") too quickly
    if (millis() - dataShowingTime < DATA_SHOWING_DURATION) return;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: " + String(temp, 2) + "C");
    lcd.setCursor(0, 1);
    lcd.print(" Humidity: " + String(humidity, 1) + "%");
}

// =============================
// Public Interfaces
// =============================

void triggerDoorBeep() {
    isBeeping = true;
    beepStartTime = millis();
}

void showTemporaryMessage(String message) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(message);
    dataShowingTime = millis();
}

void setAutoMode(bool mode) { autoMode = mode; }
void setAppFan(bool state) { appFan = state; }
void setAppLights(bool state) { appLights = state; }
void setAppHood(bool state) { appHood = state; }

bool shouldSyncLightsToCloud() {
    if (syncOutsideLightsFlag) {
        syncOutsideLightsFlag = false; // Reset the flag after reading
        return true;
    }
    return false;
}

bool getAppLightsState() {
    return appLights;
}