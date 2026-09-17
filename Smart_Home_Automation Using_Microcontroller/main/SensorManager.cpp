#include "SensorManager.h"
#include "DHTesp.h"
#include "pins.h"

// --- Protected Local Variables ---
// constexpr enforces immutability and type safety at compile time
constexpr int GAS_THRESHOLD = 2500; 
constexpr float FAN_TEMP_THRESHOLD = 35.0f;

// Static variables are completely private to this specific file
static DHTesp dhtSensor;
static bool currentDangerState = false;
static float currentTemp = 0.0;
static float currentHumidity = 0.0;

void initSensors() {
    pinMode(MQ2_PIN, INPUT);
    pinMode(Buzzer_PIN, OUTPUT);
    dhtSensor.setup(DHT22_PIN, DHTesp::DHT22);
}

void updateSensors() {
    // Read Gas
    int gasValue = analogRead(MQ2_PIN);
    currentDangerState = (gasValue >= GAS_THRESHOLD);
    digitalWrite(Buzzer_PIN, currentDangerState);

    // Read Temp
    TempAndHumidity newData = dhtSensor.getTempAndHumidity();
    if (!isnan(newData.temperature)) {
        currentTemp = newData.temperature;
        currentHumidity = newData.humidity;
    }
}

bool isGasDangerous() {
    return currentDangerState;
}

float getTemperature() {
    return currentTemp;
}

float getHumidity() {
    return currentHumidity;
}