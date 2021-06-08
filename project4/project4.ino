const int greenL = 9;
const int redL = 10;
const int blueL = 11;

const int redS = A0;
const int greenS = A1;
const int blueS = A2;

int redVal = 0;
int greenVal = 0;
int blueVal = 0;

int redSVal = 0;
int greenSVal = 0;
int blueSVal = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(greenL, OUTPUT);
  pinMode(redL, OUTPUT);
  pinMode(blueL, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  redSVal = analogRead(redS);
  delay(5);
  greenSVal = analogRead(greenS);
  delay(5);
  blueSVal = analogRead(blueS);
/*
  Serial.print("Raw sensor values \t RED: ");
  Serial.print(redSVal);
  Serial.print("\t GREEN: ");
  Serial.print(greenSVal);
  Serial.print("\t BLUE: ");
  Serial.println(blueSVal);
*/
  //adjusted values
  redVal = redSVal/32 + 5;
  greenVal = 255 - 5000/(greenSVal/4);
  blueVal = blueSVal/32 + 5;

 
  Serial.print("Mapped sensor values \t RED: ");
  Serial.print(redVal);
  Serial.print("\t GREEN: ");
  Serial.print(greenVal);
  Serial.print("\t BLUE: ");
  Serial.println(blueVal);
  

   analogWrite(redL, redVal);
   analogWrite(greenL, greenVal);
   analogWrite(blueL, blueVal);
}
