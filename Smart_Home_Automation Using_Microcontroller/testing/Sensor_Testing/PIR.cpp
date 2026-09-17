// Define pins
#define PIR_PIN 32
#define LED_PIN 16
void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
}
void loop() {
  int pirValue = digitalRead(PIR_PIN);
  if (pirValue == HIGH) {
    digitalWrite(LED_PIN, HIGH);   // Turn LED ON
    Serial.println("Motion detected!");
  } else {
    digitalWrite(LED_PIN, LOW);    // Turn LED OFF
    Serial.println("No motion");
  }
  delay(200); // Small delay for stability
}

