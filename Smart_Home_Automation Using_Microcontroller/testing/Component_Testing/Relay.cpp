void setup() {

pinMode(17, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000); // this speeds up the simulation
  digitalWrite(17, HIGH);
    delay(1000);
    digitalWrite(17, LOW);
}
