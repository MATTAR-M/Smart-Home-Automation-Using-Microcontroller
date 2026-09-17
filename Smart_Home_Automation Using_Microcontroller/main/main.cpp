#include <Arduino.h>
#include "esp_task_wdt.h"

// --- Custom Modules ---
#include "config.h"
#include "pins.h"
#include "SensorManager.h"
#include "DoorController.h"
#include "EnvironmentController.h" 
#include "CloudService.h"          

// --- Timing ---
unsigned long lastFastLoop = 0;
unsigned long lastSlowLoop = 0;
constexpr unsigned long FAST_LOOP_INTERVAL = 100;
constexpr unsigned long SLOW_LOOP_INTERVAL = 2000;

void setup() {
    // 1. Watchdog Setup
    esp_task_wdt_config_t twdt_config = {
        .timeout_ms = 30000,
        .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,
        .trigger_panic = true
    };
    esp_task_wdt_init(&twdt_config); 
    esp_task_wdt_add(NULL);

    Serial.begin(115200);

    // 2. Initialize domain modules
    initSensors();
    initDoor();
    initEnvironment(); // Sets up PIR, LDR, Fans, Hood, Buzzer, LCD
    initCloud();       // Connects WiFi and Firebase
}

void loop() {
    esp_task_wdt_reset();
    
    maintainCloudConnection(); 

    // ==========================================
    // Fast Operations (Responsiveness)
    // ==========================================
    if (millis() - lastFastLoop >= FAST_LOOP_INTERVAL) {
        lastFastLoop = millis();
        
        // 1. Check Door & RFID
        if (updateDoor()) {
            triggerDoorBeep(); 
            showTemporaryMessage(getDoorState() ? " Door is open" : " Door is closed");
            syncDoorStateToCloud(getDoorState());
        }
        
        // 2. Check Motion, LDR, and manage Buzzer state
        updateEnvironmentFast(isGasDangerous()); 

        // 3. Sync lights to Firebase if LDR changed them in Auto Mode
        if (shouldSyncLightsToCloud()) {
            syncOutsideLightsToCloud(getAppLightsState());
        }
    }

    // ==========================================
    // Slow Operations (Sensor Polling & Cloud)
    // ==========================================
    if (millis() - lastSlowLoop >= SLOW_LOOP_INTERVAL) {
        lastSlowLoop = millis();
        
        // 1. Read Gas and DHT
        updateSensors();
        
        // 2. Trigger Fans, Hood, and refresh LCD
        updateEnvironmentSlow(getTemperature(), getHumidity(), isGasDangerous()); 
        
        // 3. Upload all telemetry to Cloud
        if (isCloudReady()) {
            uploadSensorData(getTemperature(), getHumidity(), isGasDangerous());
        }
    }
}