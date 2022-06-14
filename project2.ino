#include <EEPROM.h>
#include "customChars.h"
#include "declarations.h"


#include <ShiftedLCD.h>
#include <arduino-timer.h>

auto timer = timer_create_default();
LiquidCrystal lcd(10);

void displayBigNumbers(long score, int column, int row){
  if (score < 10){
    lcd.setCursor(column,row);
    lcd.write(0 + '0');
    lcd.setCursor(column+1,row);
    lcd.write(0 + '0');
    lcd.setCursor(column+2,row);
    lcd.write(0 + '0');
    lcd.setCursor(column+3,row);
    lcd.write(score + '0');
  }
  else if (score < 100){ //Display numbers 0-99
    lcd.setCursor(column, row);
    lcd.write(0 + '0');

    lcd.setCursor(column + 1, row);
    lcd.write(0 + '0');

    lcd.setCursor(column + 2, row);
    lcd.write(score/10 + '0');
    
    lcd.setCursor(column + 3, row);
    lcd.write(score%10 + '0');
  }
  
  else if (score < 1000){ //Display numbers 100-990
    lcd.setCursor(column, row);
    lcd.write(0 + '0');

    lcd.setCursor(column + 1, row);
    lcd.write(score/100 + '0');
    lcd.setCursor(column + 2, row);
    if (score % 100 == 0){
      lcd.write(0 + '0');
      lcd.setCursor(column + 3, row);
      lcd.write(0 + '0');
    }
    else{
      lcd.write((score%100)/10 + '0');
      lcd.setCursor(column + 3, row);
      lcd.write((score%100)%10 + '0');
    }
  }

  else{ //Display numbers 1000 - 9990
    lcd.setCursor(column, row);
    lcd.write(score/1000 + '0');
    lcd.setCursor(column + 1, row);
    
    if (score%1000 < 100) {
      lcd.write(0+'0');
    }
    else{
      lcd.write((score%1000)/100 + '0');
    }
    
    lcd.setCursor(column + 2, row);
    if (score % 100 == 0){
      lcd.write(0 + '0');
      lcd.setCursor(column + 3, row);
      lcd.write(0 + '0');
    }
    else{
      lcd.write((score%100)/10 + '0');
      lcd.setCursor(column + 3, row);
      lcd.write((score%100)%10 + '0');
    }
  } 
}

enum menusState{start, displayTitle, displayMenu1, displayHighScores} menusState;
void menusTick(){
  switch(menusState){
    case start:
      lcd.setCursor(15,0);
      lcd.print(" ");
      if (gameStartFlag == 0 && titleScreenFlag == 1){
        titleScreenCnt = 0;
        menusState = displayTitle;
      }
      else{
        lcd.setCursor(16,1); //I dont know why but this fixes an error i get when i try to go back to the title screen after finishing a game
        lcd.print(" ");
        menusState = start;
      }
      break;
    case displayTitle:
      titleScreenCnt++;
      if (button2Click){
        titleScreenFlag = 0;
      }
      if (titleScreenFlag == 0){
        menusState = displayMenu1;
        pos = 1;
      }
      else{
        lcd.setCursor(0,0);
        lcd.print("   Dino Dash!");

        if (titleScreenCnt % 2 == 0){
          lcd.setCursor(0,1);
          lcd.print("  Press Start!");
          lcd.setCursor(0,1); //Display dinosaur character 1
          lcd.write(0);
          lcd.setCursor(15,1);
          lcd.write(0);
        }
        else{
          lcd.setCursor(0,1);
          lcd.print("              ");
          lcd.setCursor(0,1); //Display dinosaur character 2
          lcd.write(1);
          lcd.setCursor(15,1);
          lcd.write(1);
        }
        menusState = displayTitle;
      }
      break;
    case displayMenu1:
      lcd.clear();
      lcd.print("High-Scores    *");
      lcd.setCursor(0,1);
      lcd.print("Play           *");

      if (pos && button3Click){
        pos = 0;
      }
      else if (!pos && button1Click){
        pos = 1;
      }

      if (pos){ 
        lcd.setCursor(15,0);
        lcd.blink();
      }
      else{
        lcd.setCursor(15,1);
        lcd.blink(); 
      }

      if (pos && button2Click){ //Cursor position hovering over HIGHSCORES
        menusState = displayHighScores;
        pos = 1;
      }
      else if (!pos && button2Click){ //Cursor position hovering over PLAY
        menusState = start;
        lcd.clear();
        gameStartFlag = 1;
      }
      else{
        menusState = displayMenu1;
      }      
      break;
    case displayHighScores:
      lcd.clear();
      if (pos){
        lcd.setCursor(0,0);
        lcd.print("1-");
        
        displayBigNumbers(highScore1, 2, 0);
        //lcd.write(highScore1 + '0');
        
        lcd.setCursor(0,1);
        lcd.print("2-");
        displayBigNumbers(highScore2, 2, 1);
        //lcd.write(highScore2 + '0');
      }
      else if (!pos){
        lcd.setCursor(0,0);
        lcd.print("2-");
        displayBigNumbers(highScore2, 2, 0);
        //lcd.write(highScore2 + '0');
        
        lcd.setCursor(0,1);
        lcd.print("3-");
        displayBigNumbers(highScore3, 2, 1);
        //lcd.write(highScore3 + '0');
      }
      lcd.noCursor();
      lcd.noBlink();

      if (pos && button3Click){
        pos = 0;
      }
      else if(!pos && button1Click){
        pos = 1;
      }
      
      if (button2Click){
        menusState = displayMenu1;
        pos = 1;
      }
      else{
        menusState = displayHighScores;
      }
      break;
  }   
}

enum buttonState{start2, buttonRelease, button1Press, button2Press, button3Press} buttonState;
void buttonTick(){
  switch(buttonState){
    case start2:
      buttonState = buttonRelease;
      break;
    case buttonRelease:
      button1Click = 0;
      button2Click = 0;
      button3Click = 0;

      if (digitalRead(button1) == LOW){
        buttonState = button1Press;
        button1Click = 1;
      }
      else if (digitalRead(button2) == LOW){
        buttonState = button2Press;
        button2Click = 1;
      }
      else if (digitalRead(button3) == LOW){
        buttonState = button3Press;
        button3Click = 1;
      }
      else{
        buttonState = buttonRelease;
      }
      break;
    case button1Press:
      if (digitalRead(button1) == HIGH){
        buttonState = buttonRelease;
      }
      else{
        buttonState = button1Press;
      }
      break;
    case button2Press:
      if (digitalRead(button2) == HIGH){
        buttonState = buttonRelease;
      }
      else{
        buttonState = button2Press;
      }
      break;
    case button3Press:
      if (digitalRead(button3) == HIGH){
        buttonState = buttonRelease;
      }
      else{
        buttonState = button3Press;  
      }
      break;
  }
}

enum gameState{start3, waiting, gamePlay, gameOver} gameState;
void gameTick(){
  switch (gameState){
    case start3:
      gameState = waiting;
      break;
    case waiting:
      if (gameStartFlag == 1){
        gameState = gamePlay;
        i = 0;
        gamei = -1;
        gameScore = 0;
        dinoFrame = 0;
        dinoJump = 0; //0 = no jump, 1 = jump;
        cacPos1 = 16;
        cacPos2 = 24;
      }
      else{
        gameState = waiting;
      }
      break;
    case gamePlay:
        i++;
        if (i% 2 == 0 && gameScore < 10000){
          gameScore += 1;
        }

        if (gameScore <= 100){
          periodSpeed = 3;
        }
        else if (gameScore > 150){
          periodSpeed = 2;
        }
        else if (gameScore > 200){
          periodSpeed = 1;
        }
        gamei++;
        if (gamei > periodSpeed){
          //lcd.clear();
          lcd.setCursor(0,1);
          lcd.print(" ");
          lcd.setCursor(cacPos1,1);
          lcd.print(" ");
          lcd.setCursor(cacPos2,1);
          lcd.print(" ");
          cacPos1--;
          cacPos2--;
        
        lcd.setCursor(cacPos1,1);
        lcd.write(2);
        lcd.setCursor(cacPos2,1);
        lcd.write(3);
        
        if (cacPos1 < 0){
            //cacPos1 = 16;
            cacPos1 = cacPos2 + 10;
        }
        if (cacPos2 < 0){
          //cacPos2 = 24;
          cacPos2 = cacPos1 + 10;
        }
        gamei = -1;
      }

      if ((dinoJump == 0 && cacPos1 == 2) ||(dinoJump == 0 && cacPos2 == 2)){
          gameState = gameOver;
          gamei = 0;
          gameStartFlag = 0;
          lcd.clear();
          lcd.print("     SCORE:     ");
  
          if (gameScore >= highScore1){
            highScore3 = highScore2;
            highScore2 = highScore1;
            highScore1 = gameScore;
            EEPROM.write(4, highScore1);
            EEPROM.write(5, highScore2);
            EEPROM.write(6, highScore3);
          }
          else if (gameScore >= highScore2){
            highScore3 = highScore2;
            highScore2 = gameScore;
            EEPROM.write(5, highScore2);
            EEPROM.write(6, highScore3);
          }
          else if(gameScore >= highScore3){
            highScore3 = gameScore;
            EEPROM.write(6, highScore3);
          }
          
      }
      else{
        gameState = gamePlay;
        displayBigNumbers(gameScore, 12, 0);
      }
      break;
    case gameOver:
      gamei++;

      if (gamei % 4 == 0){
        dinoJump = !dinoJump;
        lcd.setCursor(0, !dinoJump);
        lcd.print(" ");
        lcd.setCursor(0, dinoJump);
        lcd.write(0);
        lcd.setCursor(15, !dinoJump);
        lcd.print(" ");
        lcd.setCursor(15,dinoJump);
        lcd.write(0);
      }
      
      if (gamei < 10){
        displayBigNumbers(gameScore, 6, 1);
        lcd.setCursor(16,2);
      }
      else if (gamei < 16){
        lcd.setCursor(5,1);
        lcd.print("     ");
        lcd.setCursor(16,2);
      }
      if (gamei == 16){
        gamei = 0;
      }

      if (button2Click){
        gameState = waiting;
        titleScreenFlag = 1;
        //lcd.clear(); 
        gameStartFlag = 0;
      }
      else{
        gameState = gameOver;
      }
      
      break;
    default:
      gameState = start3;
      break;
  }
}

enum dinoState{start4, waiting2, readyToJump, jump1,jump2} dinoState;
void dinoTick(){
  switch(dinoState){
    case start4:
      dinoState = waiting2;
      break;
    case waiting2:
      if (gameStartFlag == 1){
        dinoState = readyToJump;
      }
      else{
        dinoState = waiting2;
      }
      break;
    case readyToJump:
      if (gameStartFlag == 0){
        dinoState = waiting2;
      }
      else{
        //////////////
        dinoFrame = !dinoFrame;
        lcd.setCursor(2,1);
        lcd.write(dinoFrame);
        lcd.setCursor(16,2);
        //////////////
        
        if (button2Click){
          dinoJump = 1;
          dinoState = jump1;
        }
        else{
          dinoState = readyToJump;
        }
      }
      break;
    case jump1:
        if (gameStartFlag == 0){
          dinoState = waiting2;
        }
        else{
          if (!(cacPos1 == 2) && !(cacPos2 == 2)){
            lcd.setCursor(2,1);
            lcd.print(" ");
          }
          lcd.setCursor(2,0);
          lcd.write(dinoFrame);
          lcd.setCursor(16,2);
        
          dinoi++;
          if (dinoi < 3){
            dinoState = jump1;
            dinoJump = 1;
          }
          else{
            dinoState = jump2;
            dinoi = 0;
          }
          //dinoPeriodi = -1;
        }
      break;
    case jump2:
      if (gameStartFlag == 0){
         dinoState = waiting2;
      }
      else{
        lcd.setCursor(2,0);
        lcd.print(" ");
        lcd.setCursor(2,1);
        lcd.write(dinoFrame);
        lcd.setCursor(16,2);
  
        
        dinoJump = 0;
        dinoi++;
        if (dinoi < 2){
          dinoState = jump2;
        }
        else{
          dinoState = readyToJump;
          dinoi = 0;
        }
      }
      break;
  }
}
  
void setup() {
  EEPROM.write(4, highScore1);
  EEPROM.write(5, highScore2);
  EEPROM.write(6, highScore3);
  //Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.createChar(0, dino1);
  lcd.createChar(1, dino2);
  lcd.createChar(2, cactus1);
  lcd.createChar(3, cactus2);

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);  
  pinMode(button3, INPUT_PULLUP);  

  timer.every(200, menusTick);
  timer.every(50, buttonTick);
  timer.every(100, gameTick);
  timer.every(250, dinoTick);
}

void loop() {
  highScore1 = EEPROM.read(4);
  highScore2 = EEPROM.read(5);
  highScore3 = EEPROM.read(6);
  timer.tick();
}
