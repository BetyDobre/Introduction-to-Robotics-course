#include <LiquidCrystal.h>
#include <EEPROM.h>

const int D4 = 5;
const int D5 = 4;
const int D6 = 3;
const int D7 = 2;
const int RS = 12;
const int enable = 11;

const int pinX = A0;
const int pinY = A1;
const int pinSW = 7;

int xValue = 0;
int yValue = 0;
int switchValue;
int currentItem = 0;
byte check = 0;

bool pressedButton = false;
bool yJoyMoved = false;
bool xJoyMoved = false;
bool gameOver = false;

int minTreshold = 400;
int maxTreshold = 600;
int levelDelay = 5000;

int level = 0;
int score = 0;
int lives = 3;
int nr = 0;
int highscore = 0;
int startingLevel = 0;

unsigned long startTime;
unsigned long lastTime = 0;

String myName = "";
char incomingByte = 0;

LiquidCrystal lcd(RS, enable, D4, D5, D6, D7);

void displayMenu(LiquidCrystal lcd, int currentItem) {
  lcd.clear();

  lcd.print(currentItem == 0 ? ">" : " ");
  lcd.print("Play");

  lcd.print(currentItem == 2 ? ">" : " ");
  lcd.print("Highscore");

  lcd.setCursor(0, 1);

  lcd.print(currentItem == 1 ? ">" : " ");
  lcd.print("Settings");
};

void moveMainMenu() {
  yValue = analogRead(pinY);
  
  if(yValue < minTreshold && yJoyMoved == false) {
    if(currentItem < 2) {
      currentItem++;
    }
    else {
      currentItem = 0;
    }
    yJoyMoved = true;
  }
  
  if(yValue > maxTreshold && yJoyMoved == false) {
    if(currentItem > 0) {
      currentItem--;
    }
    else {
      currentItem = 2;
    }
    yJoyMoved = true;
  }
  
  if(yValue >= minTreshold && yValue <= maxTreshold) {
    yJoyMoved = false;
  }
  delay(1);
}

void levelMove() {
  xValue = analogRead(pinX);
  
  if(xValue < minTreshold && xJoyMoved == false) {
    if(startingLevel > 0) {
      startingLevel--;
    }
    else {
      startingLevel = 9;
    }
    xJoyMoved = true;
  }
  
  if(xValue > maxTreshold && xJoyMoved == false) {
    if(startingLevel < 9) {
      startingLevel++;
    }
    else {
      startingLevel = 0;
    }
    xJoyMoved = true;
  }
  
  if(xValue >= minTreshold && xValue <= maxTreshold)
      xJoyMoved = false;
  delay(1);
}

void displayMenuPlay(LiquidCrystal lcd) {
  lcd.clear();
  
  if(startTime > levelDelay && lastTime == 0) {
    long quotient = startTime / levelDelay;
    long reminder = startTime % levelDelay;
    lastTime += levelDelay * quotient + reminder;
  }
  else
    if(lastTime == 0) {
      long reminder = startTime%levelDelay;
      lastTime += reminder;
    }

  if(!gameOver) {
    lcd.print("Lives:");
    lcd.print(lives);
    lcd.print(" Level:");
    lcd.print(level);
    lcd.setCursor(0, 1);
    lcd.print("Score:");
    score = level * 3;
    lcd.print(score);
  }
  else {
    lcd.print("You finished the");
    lcd.setCursor(0, 1);
    lcd.print("game.Congrats!");
    if(score > highscore)
         highscore = score;
    EEPROM.write(0, highscore);
  }
  
  if(!gameOver && startTime - lastTime > levelDelay) { 
    level++;
    lastTime = startTime;
    nr++;
  }
  
  if(nr == 2)
    gameOver = true;
  delay(50);    
}

void displayMenuHighscore(LiquidCrystal lcd) {
  lcd.clear();
  lcd.print("Highscore:");
  lcd.print(highscore);
  delay(50);
}

void displayMenuSettings(LiquidCrystal lcd) {
  lcd.clear();
  lcd.print("Starting level:");
  
  if(!gameOver) 
    levelMove();
    
  lcd.print(startingLevel);
  level = startingLevel;
  lcd.setCursor(0, 1);
  lcd.print("Name:");
  
  if (Serial.available() > 0) {
    incomingByte = (char)Serial.read();
    if(incomingByte != '\n')
       myName += incomingByte;
  }
  
  lcd.print(myName);
  delay(50);
}

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  
  pinMode(pinSW, INPUT_PULLUP);
  
  highscore = EEPROM.read(0);
  EEPROM.write(0, 0);
  
  Serial.begin(9600);
}

void loop() {
  switchValue = digitalRead(pinSW);
  startTime = millis();
  
  if(switchValue == LOW && check == 0) {
      pressedButton = !pressedButton;
      check = 1;
  }
  
  if(switchValue == HIGH) {
    check = 0;
  }

  if(pressedButton == true)
  {
    if(currentItem == 0) {
      displayMenuPlay(lcd);
    }
    else
      if (currentItem == 2) {
          displayMenuHighscore(lcd);
      }
      else {
        displayMenuSettings(lcd);
      } 
  }
  else { 
    moveMainMenu();
    displayMenu(lcd,currentItem);
    delay(50);
  }
  
  delay(1);
}
