# Tower of Power
### How to play
  The idea of this game is that you have a fixed bar and a moving bar and you have to connect each other. How do you do that? By pushing the button exactly in the moment when they are at the same level. Imagine that you have two cables and you have to connect them to produce electricity. Seems easy, right?
  
  Initially you have three lives and every time you match the bars the score will increase by one. When you miss, you lose a life. The fixed bar is positioned randomly. When you run out of lives, you lose.
  
  As you progress, the level and the speed will increase and it will get pretty hard to play. Also, at some point the fixed bar will become a time indicator. What should this mean? It will start to blink faster and faster. The faster it blinks, the less time you have to press the button. If you don't manage it on time, you lose a life.
  
  At the end of the game, you'll be able to see if you made it to a highscore. In case of that, your name and highscore will be saved for all to see them. 
  
  In the game menu, which you scroll through with the joystick, you are able to enter your name, to set the starting level of the game(the difficulty), to see the last saved highscore, to see some info about the game(such as the name of the creator and his Github link, the game name etc.) and of course, you press "Play" to start the game. After finishing the game, you can see your score, the level you reached and press "Play again" to restart the game.
  
  This game can be found in arcades and here is a example of it: https://www.youtube.com/watch?v=43UP6-Bc-FA
### Hardware required
- 8x8 LED matrix
- LCD
- Joystick
- MAX7219 Driver
- Pushbutton
- Active buzzer
- Wires

### Technical requierments
The game is displayed on a 8x8 LED matrix using MAX7219 driver. For the control of the LCD it is used a joystick and for the game it is used a pushbutton. Some melodies are played when you start the game and when you lose/win using a buzzer. While playing, relevant info is displayed on the LCD.

The game has levels, more lives, time pressure and progress in difficulty(speed). Also, the score in savable in EEPROM.
