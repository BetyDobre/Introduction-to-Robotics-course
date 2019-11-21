const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;

const int pinSW = 1; 
const int pinX = A0; 
const int pinY = A1;

int switchValue;
int xValue = 0;
int yValue = 0;

bool xJoyMoved = false;
bool yJoyMoved = false; 
byte xDigit = 0;
byte yDigit = 0;

const int minTreshold = 400;
const int maxTreshold = 600;

const int segSize = 8;
const int noOfDisplays = 4;
const int noOfDigits = 10;

bool pressedButton = false;
int dpState = LOW;
int check = 0;

int v_dpStates[noOfDisplays] = { 
  LOW, LOW, LOW, LOW
};

int v_digits[noOfDisplays] = { 
  0, 0, 0, 0
};

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int digits[noOfDisplays] = {
  pinD1, pinD2, pinD3, pinD4
};

byte digitMatrix[noOfDigits][segSize - 1] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void displayNumber(byte digit) {
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  digitalWrite(segments[segSize - 1], dpState);
}

void showDigit(int num) {
  for (int i = 0; i < noOfDisplays; i++) {
      digitalWrite(digits[i], HIGH);
  }
  digitalWrite(digits[num], LOW);
}

void xMove() {
    xDigit = v_digits[yDigit];
    xValue = analogRead(pinX);
    if(xValue < minTreshold && xJoyMoved == false) {
      if(xDigit > 0) {
        xDigit--;
      }
      else {
        xDigit = 9;
      }
      xJoyMoved = true;
    }
    
    if(xValue > maxTreshold && xJoyMoved == false) {
      if(xDigit < 9) {
        xDigit++;
      }
      else {
        xDigit = 0;
      }
      xJoyMoved = true;
    }
    
    if(xValue >= minTreshold && xValue <= maxTreshold)
        xJoyMoved = false;
    v_digits[yDigit] = xDigit;
    delay(1);
}

void yMove() {
    yValue = analogRead(pinY);
    if(yValue < minTreshold && yJoyMoved == false) {
      if(yDigit < 3) {
            yDigit++;
      }
      else {
        yDigit = 0;
      }
        yJoyMoved = true;
    }
    
    if(yValue > maxTreshold && yJoyMoved == false) {
      if(yDigit > 0) {
            yDigit--;
      }
      else {
        yDigit = 3;
      }
      yJoyMoved = true;
    }
    
    if(yValue >= minTreshold && yValue <= maxTreshold) {
      yJoyMoved = false;
    }
    
    for(int i = 0; i < noOfDisplays; i++) {
      v_dpStates[i] = LOW;
    }
    
    v_dpStates[yDigit] = HIGH;
    delay(1);
}

void setup() {
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  
  for (int i = 0; i < noOfDisplays; i++) {
    pinMode(digits[i], OUTPUT);
  }
  
  pinMode(pinSW, INPUT_PULLUP);
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
  
  if(pressedButton == true) {
    xMove();
    delay(1);
  }
  else {
    yMove();
    delay(1);
  }

  for (int i = 0; i < noOfDisplays; i++) {
      dpState = v_dpStates[i];
      showDigit(i);
      displayNumber(v_digits[i]);
      delay(1);
   }
}
