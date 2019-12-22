#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "LedControl.h"
#include "notes.h"

LedControl lc = LedControl(12, 11, 10, 1);

// LCD pins
const int D4 = 6;
const int D5 = 7;
const int D6 = 5;
const int D7 = 13;
const int RS = 8;
const int enable = 9;

// joystick pins
const int pinX = A1;
const int pinY = A0;
const int pinSW = 2;

LiquidCrystal lcd(RS, enable, D4, D5, D6, D7);

const int pushButton = 4;         // pushbutton pin
const int barNum = 4;             // number of leds for a bar

bool winGame = false;
bool gameOver = false;
bool clearLCD = false;            // LCD state, whether the LCD should be updated or not
bool firstTimeLcd = false;        // displaying the LCD when the game starts
bool optionPlay = false;          // variable for the second menu when the game is over
bool increaseBarLevel = true;     // positions(levels) of the moving bar         
bool fixedBarState = true;        

int previousScore = 0;            // saved to see if we can increase the speed of the moving bar
int score = 0;
int highscore = 0;
int level = 1;
int lives = 3;
int startingLevel = 1;
int currentItem = 0;             // index for the main menu items
int gameOverItem = 0;            // index for the game over menu(play again and menu) items
int speedOfBar = 900;            // stores speed of the moving bar
int interval = 1000;             // time required to have passed to check if the speed can be increased since the last increase
int speedStart = 100                  // used to calculate the speed of the moving bar using the starting level
int fixedRow = 0;                // row of the fixed bar index
int timePressure = 300;
int startBlinkSpeed = 500;
int screenInfo = 0;              // used to show the correct screen on the Info menu
int buttonState = 0;             // check if the pushbutton was pressed or not
int randNumber = 0;              // fixed bar positioned randomly
int previousRandNumber = 0;      // stored to turn off previous fixed bar

unsigned long progressTime = 0;           // time since the game started
unsigned long delayDescrease = 0;   
unsigned long blinkMillis = 0;            // used to measure the time since the last blink

char incomingByte = 0;
String playerName = "";         // stores the player name from the Serial monitor
String bestPlayer = "";         // name of the player who made it to highscore

// joystick move variables
int xValue = 0;
int yValue = 0;
int switchValue;
int swGameOverMenu;
int minTreshold = 400;
int maxTreshold = 600;
byte check = 0;
byte checkGameOverMenu = 0;
bool pressedButton = false;
bool pressedButtonGameOverMenu = false;
bool yJoyMoved = false;
bool xJoyMoved = false;

byte smile[8] = {0x3C, 0x42, 0xA5, 0x81, 0xA5, 0x99, 0x42, 0x3C};
byte sad[8] = {0x3C, 0x42, 0xA5, 0x81, 0x99, 0xA5, 0x42, 0x3C};
byte hi[] = {B00000010, B00000000, B01001010, B01001010, B01111010, B01001010, B01001010, B00000000};

void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pushButton, INPUT);

  lc.shutdown(0, false);
  lc.setIntensity(0, 2); 
  lc.clearDisplay(0);
  randomSeed(analogRead(0));
  
  progressTime = millis();
  
  lcd.begin(16, 2);
  lcd.clear();
  introductionToGame();
  displayMenu(lcd, currentItem);

  fixedBar();
  
  highscore = EEPROM.read(0);
  bestPlayer = EEPROM.read(1);
  //EEPROM.write(0, 0);         // the highscore  initialized with 0 in the beggining
  //EEPROM.write(1, 0);         // the name of the best player intialized with null in the beggining
  
  Serial.begin(9600);
}

void introductionToGame() {
  int tempo = 225;
  int notes = sizeof(startMelody) / sizeof(startMelody[0]) / 2;
  int wholenote = (60000 * 4) / tempo;
  int divider = 0;
  int noteDuration = 0;
  
  for (int j = 0; j < notes * 2; j = j + 2) {
    printByte(hi);
    
    lcd.clear();
    lcd.print("    Welcome!");
    lcd.setCursor(0, 1);
    lcd.print("   Good luck!");
    
    divider = pgm_read_word_near(startMelody + j + 1);
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } 
    else 
      if (divider < 0) {
        noteDuration = (wholenote) / abs(divider);
        noteDuration *= 1.5;
      }
    
    tone(buzzerPin, pgm_read_word_near(startMelody + j), noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzerPin);
  }
  
  lc.clearDisplay(0);
}

void displayMenu(LiquidCrystal lcd, int currentItem) {
  lcd.clear();
  
  lcd.print(currentItem == 0 ? ">" : " ");
  lcd.print("Play");

  lcd.print(currentItem == 3 ? ">" : " ");
  lcd.print("Highscore");

  lcd.setCursor(0, 1);

  lcd.print(currentItem == 2 ? ">" : " ");
  lcd.print("Settings");

  lcd.print(currentItem == 1 ? ">" : " ");
  lcd.print("Info");
  
  switchValue = digitalRead(pinSW);
}

void displayMenuPlay(LiquidCrystal lcd) {
  if(!firstTimeLcd) {
    lcd.clear();
    
    lcd.print("Lives:");
    lcd.print(lives);
    
    lcd.print(" Score:");
    lcd.print(score);
    
    lcd.setCursor(0, 1);
    
    lcd.print("Level:");
    lcd.print(level);
    
    firstTimeLcd = true;
  }
  
  move();
  
  if(clearLCD) {
    lcd.clear();
    
    lcd.print("Lives:");
    lcd.print(lives);
    
    lcd.print(" Score:");
    lcd.print(score);
    
    lcd.setCursor(0, 1);
    lcd.print("Level:");
    
    lcd.print(level);
    
    clearLCD = false;
  }
}

void displayMenuHighscore(LiquidCrystal lcd) {
  lcd.clear();
  
  lcd.print("Highscore:");
  lcd.print(highscore);

  lcd.setCursor(0, 1);
  lcd.print("Name:");
  String l = read_String(1);
  lcd.print(l); 
  
  delay(50);
}

void displayMenuSettings(LiquidCrystal lcd) {
  lcd.clear();
  
  lcd.print("Starting level:");
  
  if(!gameOver) {
    moveStartingLevel();
  }
    
  lcd.print(startingLevel);
  level = startingLevel;
  
  lcd.setCursor(0, 1);
  
  lcd.print("Name:");
  if (Serial.available() > 0) {
    incomingByte = (char)Serial.read();
    if(incomingByte != '\n')
       playerName += incomingByte;
  }
  
  lcd.print(playerName);
  
  delay(50);
}

void displayMenuInfo(LiquidCrystal lcd) {
  xValue = analogRead(pinX);
  
  if(xValue < minTreshold && xJoyMoved == false) {
    if(screenInfo > 0) {
      screenInfo--;
    }
    else {
      screenInfo = 3;
    }
    xJoyMoved = true;
  }
  
  if(xValue > maxTreshold && xJoyMoved == false) {
    if(screenInfo < 3) {
      screenInfo++;
    }
    else {
      screenInfo = 0;
    }
    xJoyMoved = true;
  }
  
  if(xValue >= minTreshold && xValue <= maxTreshold) {
      xJoyMoved = false;
  }
  
  delay(1);

  if(screenInfo == 0) {
    lcd.clear();
    lcd.print("    Creator:   ^");
    lcd.setCursor(0, 1);
    lcd.print("Dobre Beatrice v");
  }
  else 
    if(screenInfo == 1) {
      lcd.clear();
      lcd.print("   Game name:  ^");
      lcd.setCursor(0, 1);
      lcd.print("Tower of Power v");
    }
  else  
    if(screenInfo == 2) {
      lcd.clear();
      lcd.print("  Github link: ^");
      lcd.setCursor(0, 1);
      lcd.print("https://github.>");
      
      yValue = analogRead(pinY); 
      if(yValue > maxTreshold) {  
        lcd.clear();
        lcd.print("  Github link: ^");
        lcd.setCursor(0, 1);
        lcd.print("com/BetyDobre  v");
        delay(1000);
        yJoyMoved = true;
       } 
    }
    else 
      if(screenInfo == 3) {
        lcd.clear();
        lcd.print("  Powered by:  ^");
        lcd.setCursor(0, 1);
        lcd.print("@UnibucRoboticsv");
      }
      
  delay(50);   
}

void displayGameOver(LiquidCrystal lcd, int gameOverItem) {
  if(!optionPlay) {
    if(score == highscore) {
      lcd.clear();
      lcd.print("Congrats");
      if(playerName !=  "") {
        lcd.print(",");
        lcd.print(playerName);
      }
      lcd.print("!");
      lcd.setCursor(0, 1);
      lcd.print("Highscore ");
      lcd.print(highscore);
      delay(3000);
    }
    lcd.clear();
    lcd.print("Game over! Wanna");
    lcd.setCursor(0, 1);
    lcd.print("try again?");
    delay(3000);
    optionPlay = true;
  }  
  
  lcd.clear();
  
  lcd.print("Scored:"); 
  lcd.print(score);
  
  lcd.print(" Level:");
  lcd.print(level);
  
  lcd.setCursor(0, 1);
  
  lcd.print(gameOverItem == 0 ? ">" : " ");
  lcd.print("Play again");

  lcd.print(gameOverItem == 1 ? ">" : " ");
  lcd.print("Menu");
  
  moveGameOverMenu();
}

void moveMainMenu() {
  yValue = analogRead(pinY);
  
  if(yValue < minTreshold && yJoyMoved == false) {
    if(currentItem < 3) {
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
      currentItem = 3;
    }
    yJoyMoved = true;
  }
  
  if(yValue >= minTreshold && yValue <= maxTreshold) {
    yJoyMoved = false;
  }
  
  delay(1);
}

void moveGameOverMenu() {
  yValue = analogRead(pinY);
  
  if(yValue < minTreshold && yJoyMoved == false) {
    if(gameOverItem < 1) {
      gameOverItem++;
    }
    else {
      gameOverItem = 0;
    }
    yJoyMoved = true;
  }
  
  if(yValue > maxTreshold && yJoyMoved == false) {
    if(gameOverItem > 0) {
      gameOverItem--;
    }
    else {
      gameOverItem = 1;
    }
    yJoyMoved = true;
  }
  
  if(yValue >= minTreshold && yValue <= maxTreshold) {
    yJoyMoved = false;
  }
  
  delay(1);
}

void moveStartingLevel() {
  xValue = analogRead(pinX);
  
  if(xValue > maxTreshold && xJoyMoved == false) {
    if(startingLevel > 1) {
      startingLevel--;
    }
    else {
      startingLevel = 9;
    }
    xJoyMoved = true;
  }
  
  if(xValue < minTreshold && xJoyMoved == false) {
    if(startingLevel < 9) {
      startingLevel++;
    }
    else {
      startingLevel = 1;
    }
    xJoyMoved = true;
  }
  
  if(xValue >= minTreshold && xValue <= maxTreshold)
      xJoyMoved = false;
      
  speedOfBar = interval -  startingLevel * speedStart; 
  
  delay(1);
}

void fixedBar() {
  for(int i = 0; i < barNum; i++) {
    lc.setLed(0, i, previousRandNumber, false);
  }
  
  randNumber = random(0, 8);
  
  for(int i = 0; i < barNum; i++) {
    lc.setLed(0, i, randNumber, true);
  }
  
  previousRandNumber = randNumber;
}

void movingBar() {
  if(fixedRow < 7 && increaseBarLevel == true) {
    for(int j = barNum; j < 8; j++)
      lc.setLed(0, j, fixedRow, false);
      
    fixedRow++; 
    
    for(int j = barNum; j < 8; j++)
      lc.setLed(0, j, fixedRow, true);
  }
  else {
    increaseBarLevel = false;
    
    for(int j = barNum; j < 8; j++)
      lc.setLed(0, j, fixedRow, false);
      
    fixedRow--; 
    
    for(int j = barNum; j < 8; j++)
      lc.setLed(0, j, fixedRow, true);
      
    if(fixedRow == 0) {
      increaseBarLevel = true;
    }
  }
}

void move() {
  if(winGame) {
    lc.clearDisplay(0);
    printByte(smile);
    winTone();
    
    lcd.clear();
    
    lcd.print("You finished");
    lcd.setCursor(0, 1);
    lcd.print("the game!");
    
    delay(3000);
  }
  else 
    if(gameOver) {
      printByte(sad);
      loseTone();
      displayGameOver(lcd, gameOverItem);    
    }
    else {
        if(millis() - progressTime >= speedOfBar) {
          movingBar();
          progressTime = millis();
        }
        if(millis() - delayDescrease >= interval && previousScore < score) {
          if (speedOfBar > 100) {
            speedOfBar -= 100;
          }
          else 
            if(speedOfBar > 0) {
              speedOfBar -= 10;
            }
            else {
              winGame = true;
            }
          delayDescrease = millis();
        }
      
        buttonState = digitalRead(pushButton);
        previousScore = score;
        
        if(buttonState) {    
            if(randNumber == fixedRow) {
              score++;
              timePressure = 300;
              clearLCD = true;
              
              if(score >= highscore) {
                highscore = score;
                
                if(playerName != "") {
                  bestPlayer = playerName;
                }
                else {
                  bestPlayer = "- ";
                }
              }
              
              printByte(smile);
              tone(buzzerPin, 250, 100);
              delay(500);
              
              if (score % 3 == 0) {
                level++;
              }
            }
            else {
              lives--;
              clearLCD = true;
              tone(buzzerPin, 100, 100);
              
              printByte(sad);
              delay(500);
            }
            lc.clearDisplay(0);
            fixedBar();
      }
      if(speedOfBar <= startBlinkSpeed && !gameOver) {
        if(millis() - blinkMillis >= timePressure) {
          fixedBarState = !fixedBarState;
          
          for(int i = 0; i < barNum; i++) {
            lc.setLed(0, i, randNumber, fixedBarState);
          }
          
          blinkMillis = millis();
          timePressure -= 5;
        }
        if (timePressure == 0) { 
          lives--;
          fixedBar(); 
          timePressure = 300;
          clearLCD = true;
          
          tone(buzzerPin, 100, 100);
          printByte(sad);
          delay(500);
          
          lc.clearDisplay(0);
        }
      }
    }
  if(lives == 0 && !gameOver) {
    lc.clearDisplay(0);
    EEPROM.write(0, highscore);
    writeString(1, bestPlayer);
    gameOver = true;
  }
}

void playAgain() {
  lives = 3;
  score = 0;
  previousScore = 0;
  level = 1;
  startingLevel = 1;
  speedOfBar = 900;
  firstTimeLcd = false;
  gameOver = false;
  winGame = false;
  optionPlay = false;
  oneTime = false;
  playerName = "";
  lc.clearDisplay(0);
  fixedBar();
}

void printByte(byte character[]) {
  for(int i = 0; i < 8; i++) {
    lc.setColumn(0, i, character[i]);
  }
}

void writeString(char character, String nameData) {
  int _size = nameData.length();
  int i;
  
  for(i = 0; i < _size; i++) {
    EEPROM.write(character + i, nameData[i]);
  }
  
  EEPROM.write(character + _size, '\0'); 
}
 
String read_String(char character) {
  int i;
  int len = 0;
  char nameData[20];
  unsigned char j;
  
  j = EEPROM.read(character);
  
  while(j != '\0' && len < 20) {    
    j = EEPROM.read(character + len);
    nameData[len] = j;
    len++;
  }
  
  nameData[len] = '\0';
  return String(nameData);
}

void loop() {
  switchValue = digitalRead(pinSW);
  
  if(switchValue == LOW && check == 0) {
      pressedButton = !pressedButton;
      check = 1;
  }  
  
  if(switchValue == HIGH) {
    check = 0;
  }
  
  if(pressedButton == true)
  {
    if(currentItem == 1) {
       displayMenuInfo(lcd);
    }
    else 
      if (currentItem == 2) {
        displayMenuSettings(lcd);
      }
      else
        if (currentItem == 3) {
          displayMenuHighscore(lcd);
        }
        else {
          displayMenuPlay(lcd);
        }
  }
  else { 
    if(gameOver) {
      swGameOverMenu = digitalRead(pinSW);
      
      if(swGameOverMenu == LOW && checkGameOverMenu == 0) {
          pressedButtonGameOverMenu = !pressedButtonGameOverMenu;
          checkGameOverMenu = 1;
      }  
      
      if(swGameOverMenu == HIGH) {
        checkGameOverMenu = 0;
      }
      
      if(pressedButtonGameOverMenu == true) {
        if(gameOverItem == 0) {
             playAgain();
        }
      }
      firstTimeLcd = false;
    }
    
    moveMainMenu();
    displayMenu(lcd, currentItem);
    firstTimeLcd = false;
    xJoyMoved = false;
    
    delay(50);
  } 
}
