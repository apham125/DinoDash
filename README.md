# Dino Dash

## Demo Link
https://drive.google.com/file/d/1SRSbSN5CtSufQcgzh3hlGW_mOhYDEjvv/view?usp=sharing

## Purpose
The purpose of my project is to create the google dinosaur jump game on an lcd screen. I wanted to be able to create a game just like the google game from scratch using the arduino uno, the lcd screen and buttons.

## Functionality 
The game is displayed on a 16x2 lcd screen and is controlled by 3 buttons. The three buttons are the “up”, “select”, and “down” buttons. The game starts on the title screen. When the “select” button is clicked then the screen navigates to the menu screen.

On this screen, you can pick between an “High Score” option and the “Play” option. You can choose between the two options by using the “up” and “down” buttons to move the cursor to select an option. When the “select” button is clicked while the cursor is next to the “High Score” option, the lcd navigates to the score menu. This menu shows the top 3 highest scores and can be scrolled through using the “up” and “down” buttons”. Pressing the “select” button takes you back to the previous menu.

Clicking the “select” button when the cursor is next to the “Play” option starts the game. A dinosaur avatar appears on the left side of the screen while cacti appear from the right and start moving towards the dinosaur. The score is displayed on the top right of the screen and increases by 1 every 200 ms. To jump over the cacti, you press the “select” button. The cacti speeds up when the score passes 100, 150, and 200 points. When the dinosaur collides with a cactus the game ends. The game displays your score. When you press the “select” button one more time you are redirected back to the title screen.
