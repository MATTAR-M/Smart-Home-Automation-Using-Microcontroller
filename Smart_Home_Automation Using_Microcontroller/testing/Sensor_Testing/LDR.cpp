#define LIGHT_SENSOR_PIN 33 // ESP32 pin GIOP36 (ADC0)
#define LED_PIN 16
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // reads the input on analog pin (value between 0 and 4095)
  int analogValue = 4095-analogRead(LIGHT_SENSOR_PIN);

  Serial.print("Analog Value = ");
  Serial.print(analogValue);   // the raw analog reading

  // We'll have a few threshholds, qualitatively determined
  if (analogValue < 40) {
    Serial.println(" => Dark");
    digitalWrite(LED_PIN, HIGH);
  } else if (analogValue < 800) {
    Serial.println(" => Dim");
    digitalWrite(LED_PIN, HIGH);
  } else if (analogValue < 2000) {
    Serial.println(" => Light");
    digitalWrite(LED_PIN, LOW);
  } else if (analogValue < 3200) {
    Serial.println(" => Bright");
    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.println(" => Very bright");
    digitalWrite(LED_PIN, LOW);
  }

  delay(500);
}
