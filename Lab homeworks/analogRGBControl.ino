const int potPin0 = A0;
const int potPin1 = A1;
const int potPin2 = A2;

const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

int potValue0 = 0;
int potValue1 = 0;
int potValue2 = 0;

int ledValue0 = 0;
int ledValue1 = 0;
int ledValue2 = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(potPin0, INPUT);
  pinMode(potPin1, INPUT);
  pinMode(potPin2, INPUT);
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  potValue0 = analogRead(potPin0);
  potValue1 = analogRead(potPin1);
  potValue2 = analogRead(potPin2);
  
  ledValue0 = map(potValue0, 0, 1023, 0, 255);
  ledValue1 = map(potValue1, 0, 1023, 0, 255);
  ledValue2 = map(potValue2, 0, 1023, 0, 255);
  
  setColor(ledValue0, ledValue1, ledValue2);
}

void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
