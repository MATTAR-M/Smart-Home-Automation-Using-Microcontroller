#include "CloudService.h"
#include "config.h"

// Firebase & WiFi Libraries
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Domain Controllers
#include "DoorController.h"
#include "EnvironmentController.h" 

// --- Private Objects ---
static FirebaseData fbdo, fbdo_door, fbdo_stream;
static FirebaseAuth auth;
static FirebaseConfig configData;

static void connectWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED && attempt < 40) {
        delay(500);
        attempt++;
    }
}

static void streamTimeoutCallback(bool timeout) {}

// The callback listens to Firebase and commands the local hardware
static void streamCallback(FirebaseStream data) {
    String path = data.dataPath();

    if (path == "/autoMode") setAutoMode(data.boolData());
    else if (path == "/roomFan") setAppFan(data.boolData());
    else if (path == "/outsideLights") setAppLights(data.boolData());
    else if (path == "/kitchenHood") setAppHood(data.boolData());
    else if (path == "/mainDoorLocked") {
        bool newDoorOpen = !data.boolData();
        if (newDoorOpen != getDoorState()) {
            setDoorState(newDoorOpen); 
            showTemporaryMessage(newDoorOpen ? " Door is open" : " Door is closed");
        }
    }
}

void initCloud() {
    connectWiFi();

    configData.api_key = API_KEY;
    configData.database_url = DATABASE_URL;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    Firebase.begin(&configData, &auth);
    Firebase.reconnectWiFi(true);

    Firebase.RTDB.beginStream(&fbdo_stream, "/smart_home/devices");
    Firebase.RTDB.setStreamCallback(&fbdo_stream, streamCallback, streamTimeoutCallback);
}

void maintainCloudConnection() {
    if (WiFi.status() != WL_CONNECTED) connectWiFi();
}

bool isCloudReady() {
    return Firebase.ready();
}

void uploadSensorData(float temp, float humidity, bool isGasDangerous) {
    FirebaseJson json;
    json.set("temperature", temp);
    json.set("humidity", humidity);
    json.set("doorState", getDoorState() ? "Unlocked" : "Locked");
    json.set("gasSafe", !isGasDangerous);

    Firebase.RTDB.updateNode(&fbdo, "/smart_home/sensors", &json);
}

void syncDoorStateToCloud(bool isDoorOpen) {
    if (Firebase.ready()) {
        Firebase.RTDB.setBool(&fbdo_door, "/smart_home/devices/mainDoorLocked", !isDoorOpen);
    }
}

void syncOutsideLightsToCloud(bool lightsOn) {
    if (Firebase.ready()) {
        Firebase.RTDB.setBool(&fbdo, "/smart_home/devices/outsideLights", lightsOn);
    }
}