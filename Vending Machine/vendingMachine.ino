#include <AFMotor.h>
#include <LiquidCrystal.h>
#include <Servo.h>

AF_DCMotor DCMotor1(1);
AF_DCMotor DCMotor2(2);
AF_DCMotor DCMotor3(3);
AF_DCMotor DCMotor4(4);
Servo servoMotor;

//servo
const int servoPin = 13;

// LCD pins
const int RS = 24;
const int enable = 7;
const int d4 = 5;
const int d5 = 23;
const int d6 = 3;
const int d7 = 22;

//LCD variable
int currentItem = 0;

// joystick pins
const int pinX = A8;
const int pinY = A9;
const int pinSW = 53;

// joystick variables
int xValue = 0;
int switchValue = 0;
int minTreshold = 400;
int maxTreshold = 600;
byte check = 0;
bool pressedButton = false;
bool joyMoved = false;

// ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;
long duration;
int distance;
int distanceTreshold = 5;

int moneyIn = false;
int firstProduct = 1;
int secondProduct = 2;
int thirdProduct = 1;
int fourthProduct = 2;
int insertedCoins = 0;
bool coinsCheck = false;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);                

void itemChoose() {
  xValue = analogRead(pinX);
  if(xValue < minTreshold && joyMoved == false) {
      if(currentItem > 0) {
        currentItem--;
      }
      else {
        currentItem = 3;
      }
      joyMoved = true;
  }
  if(xValue > maxTreshold && joyMoved == false) {
      if(currentItem < 3) {
        currentItem++;
      }
      else {
        currentItem = 0;      
      }
      joyMoved = true;
  }

  if(xValue >= minTreshold && xValue <= maxTreshold)
    joyMoved = false;
  displayMenu();
}

void displayMenu() {
  lcd.clear();
  
  lcd.print(currentItem == 0 ? ">" : " ");
  lcd.print("1");

  lcd.print(currentItem == 1 ? ">" : " ");
  lcd.print("2");

  lcd.setCursor(0, 1);

  lcd.print(currentItem ==  2 ? ">" : " ");
  lcd.print("3");

  lcd.print(currentItem == 3 ? ">" : " ");
  lcd.print("4");
  delay(1);
}

void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  
  DCMotor1.setSpeed(100);
  DCMotor2.setSpeed(100);
  DCMotor3.setSpeed(100);
  DCMotor4.setSpeed(10);

  lcd.begin(16, 2);
  lcd.print("Introduceti o");
  lcd.setCursor(0, 1);
  lcd.print("moneda!");

  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);

  servoMotor.attach(servoPin);
  servoMotor.write(110);
  
  Serial.begin(9600);
}

void loop() {
  // calcularea distantei 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034/2;
//  Serial.print("Distance: ");
//  Serial.println(distance);

  if(distance < distanceTreshold) {
    moneyIn = true;
    insertedCoins++;
    lcd.clear();
    lcd.print("   Alege item!");
    delay(3000);
  }
  
   if(moneyIn) {
    itemChoose();
   }
    
  switchValue = digitalRead(pinSW);
  
  if(switchValue == LOW && check == 0) {
      pressedButton = !pressedButton;
      check = 1;
  }  
  
  if(switchValue == HIGH) {
    check = 0;
  }
  
  if(pressedButton == true) {
    int price = 0;
    bool motor1 = false;
    bool motor2 = false;
    bool motor3 = false;
    bool motor4 = false;
    
    if(currentItem == 0) {
      if(insertedCoins >= firstProduct) {
        coinsCheck = true;
        price = firstProduct;
        motor1 = true;
      }
    }
    
    if(currentItem == 1) {
      if(insertedCoins >= secondProduct) {
        coinsCheck = true;
        price = secondProduct;
        motor2 = true;
      }
    }
    
    if(currentItem == 2) {
      if(insertedCoins >= thirdProduct) {
        coinsCheck = true;
        price = thirdProduct;
        motor3 = true;
      }
    }
    
    if(currentItem == 3) {
      if(insertedCoins >= fourthProduct) {
        coinsCheck = true;
        price = fourthProduct;
        motor4 = true;
      }
    }
    
    if(coinsCheck) {
      servoMotor.write(190);
      delay(1000);
      
      if(motor1) {
        DCMotor1.run(BACKWARD);
        delay(150);
        DCMotor1.run(RELEASE);
      }

      if(motor2) {
        DCMotor2.run(FORWARD);
        delay(1000);
        DCMotor2.run(RELEASE);
      }
      
      if(motor3) {
        DCMotor3.run(FORWARD);
        delay(200);
        DCMotor3.run(RELEASE);
      }

      if(motor4) {
          DCMotor4.run(BACKWARD);
      }
      
      if(!motor4) {
        delay(2000);
        servoMotor.write(110);
      }

      pressedButton = false;
      
      if(!motor4) {
        lcd.clear();
        lcd.print("   Pofta buna !");
        delay(1000);
      }
      else {
        lcd.clear();
        lcd.print("   Pofta buna !");
        delay(200);
      }
      
      lcd.clear();
      lcd.print("Introduceti o");
      lcd.setCursor(0, 1);
      lcd.print("moneda!");
      coinsCheck = false;
      insertedCoins = insertedCoins - price;
      if(insertedCoins == 0)
        moneyIn = false;
    }
    else {
      lcd.clear();
      lcd.print("Suma(");
      lcd.print(0.50 * insertedCoins);
      lcd.print(")");
      lcd.setCursor(0, 1);
      lcd.print("insuficienta!");
      delay(100);
    }
  }

  DCMotor4.run(RELEASE);
  servoMotor.write(110);
}
