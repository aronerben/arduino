const int sensorPin = A2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorVal = analogRead(sensorPin);
  Serial.print("Sensor val: ");
  Serial.print(sensorVal);
  float volt = (sensorVal/1024.0) * 5.0;
  Serial.print(", Volts: ");
  Serial.print(volt);
  float temp = (volt - 0.5) * 100.0;
  Serial.print(", degrees C: ");
  Serial.println(temp);
  
  delay(10);
}

