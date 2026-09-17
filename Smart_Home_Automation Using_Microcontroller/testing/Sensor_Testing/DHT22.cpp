  #include "DHTesp.h"


  DHTesp dhtSensor;

  void setup() {
    Serial.begin(115200);
    
    // sensor setup
    dhtSensor.setup(27, DHTesp::DHT22);
    Serial.println("DHT22 Test Started!");
  }

  void loop() {
    // read temp and humidity
    TempAndHumidity data = dhtSensor.getTempAndHumidity();

    // Making sure there is no reading mistakes
    if (dhtSensor.getStatus() != 0) {
      Serial.println("Error reading DHT22: " + String(dhtSensor.getStatusString()));
    } else {
      Serial.print("Temperature: " + String(data.temperature, 2) + " °C \t");
      Serial.println("Humidity: " + String(data.humidity, 1) + " %");
    }

    // delay 2 seconds between each data read
    delay(2000); 
  }