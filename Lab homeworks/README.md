## analogRGBControl 
  The scope of this homework is to use three potentiometers in order to directly control the colors of an RGB LED. For this I wired the potentiometers to analog input pins A0, A1 and A2, and RGB LED to analog output pins 9, 10 and 11. Then I needed to map the read values from input because these are between 0..1023 and the written values are between 0..255.
## knockDetectorBuzzer
  In this homework, the passive buzzer is used to detect a knock. Five seconds after the knock was detected, the active buzzer stars to play a song. The song is stopped using a pushbutton when you choose to press it.
## joystickDigitsControl
  The task for this homework is to control each digit of the 4 digits 7 segment display with the joystick in the following way: the X-asis is used to cycle through the digits and you can choose one by pressing the button. The DP led of the current position is on. After that, the Y-axis is used to modify the value of the digit, both up and down. By pressing the button again, you unlock the selection and be able to cycle through the 4 digits again.
## gameMenuOnLCD
  This is a prototype of a game menu with basic options: score and highscore, level and starting level, name of the player and lives. Highscore is stored on the EEPROM which allows us to track it after restarting the Arduino. 
  Here is a demo: https://www.youtube.com/watch?v=NmKovHeEeHY&feature=youtu.be
