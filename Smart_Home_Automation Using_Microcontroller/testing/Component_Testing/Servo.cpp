#include <Wire.h>
#include <ESP32Servo.h>

// --- PIN DEFINITIONS ---
#define BUTTON_PIN 15
#define SERVO_PIN 18

// --- OBJECT INITIALIZATION ---
Servo doorServo;

void setup() {
  Serial.begin(115200);
  
  // Initialize Pins
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  doorServo.attach(SERVO_PIN);
  doorServo.write(0); // Door closed
    
  delay(1000);
  Serial.print("System Active");
}

void loop() {
  // 1. Read Sensors
bool buttonPressed = !digitalRead(BUTTON_PIN); // Pull-up means LOW is pressed

  // 4. Door Control (RFID or Button)

  if (buttonPressed) {
    Serial.println("Access Granted!");
    doorServo.write(90); // Open door
    delay(5000);         // Keep open for 3 seconds
    doorServo.write(0);  // Close door
  }

  delay(1); // Short delay for stability
}