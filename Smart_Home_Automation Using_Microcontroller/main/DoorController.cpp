#include "DoorController.h"
#include "pins.h"
#include <ESP32Servo.h>
#include <SPI.h>
#include <MFRC522.h>

// --- Private Hardware Objects ---
static Servo doorServo;
static MFRC522 rfid(SDA_PIN, RST_PIN);

// --- Protected Local Variables ---
constexpr unsigned long DEBOUNCE_DELAY = 300;
constexpr unsigned long DOOR_COOLDOWN = 3000;
constexpr int SERVO_INTERVAL = 15;

static bool isDoorOpen = false;
static bool lastButtonState = HIGH;
static unsigned long lastDebounceTime = 0;
static unsigned long lastDoorActionTime = 0;

static int currentAngle = 60;
static int targetAngle = 60;
static unsigned long lastServoUpdate = 0;

void initDoor() {
    doorServo.attach(Servio_PIN);
    doorServo.write(currentAngle);
    
    pinMode(D_Button_PIN, INPUT_PULLUP);
    
    // Initialize RFID on the specified SPI pins
    SPI.begin(14, 12, 13, SDA_PIN);
    rfid.PCD_Init();
}

void setDoorState(bool open) {
    isDoorOpen = open;
    targetAngle = isDoorOpen ? 170 : 55;
    lastDoorActionTime = millis();
}

bool getDoorState() {
    return isDoorOpen;
}

bool updateDoor() {
    bool stateChanged = false;

    // 1. Handle Smooth Servo Movement (Non-blocking)
    if (millis() - lastServoUpdate >= SERVO_INTERVAL) {
        lastServoUpdate = millis();
        if (currentAngle < targetAngle) currentAngle++;
        else if (currentAngle > targetAngle) currentAngle--;
        
        doorServo.write(currentAngle);
    }

    // 2. Check Button & RFID Access
    bool currentButtonState = digitalRead(D_Button_PIN);
    bool cardDetected = rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial();
    bool trigger = false;

    if (currentButtonState == LOW && lastButtonState == HIGH) {
        if (millis() - lastDebounceTime > DEBOUNCE_DELAY) {
            trigger = true;
            lastDebounceTime = millis();
        }
    }

    if (cardDetected) {
        trigger = true;
        rfid.PICC_HaltA(); // Halt PICC to prevent rapid re-triggering
    }

    lastButtonState = currentButtonState;

    // 3. Apply changes if triggered and cooldown has passed
    if (trigger && (millis() - lastDoorActionTime >= DOOR_COOLDOWN)) {
        setDoorState(!isDoorOpen);
        digitalWrite(Buzzer_PIN, HIGH); // Replicating your original buzzer trigger
        stateChanged = true;
    }

    return stateChanged;
}