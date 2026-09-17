#define  MQ2_PIN 26
#define LED_PIN 2

//sensor setup
void setup() {
  Serial.begin(115200);
  pinMode(MQ2_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

//reading loop check for the bool of the gas level
void loop() {
  int gasvalue = analogRead(MQ2_PIN);
  Serial.print("Gas Sensor Value: ");
  Serial.print(gasvalue);
  if(gasvalue>=700){
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Danger ! Gas leak Detected!");
  }
  else{
    digitalWrite(LED_PIN, LOW);
    Serial.println(" Environment safe");
  }
  // 2 seconds delay between each read
  delay(2000); 
}

