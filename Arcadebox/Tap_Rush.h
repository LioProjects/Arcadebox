#include "Game.h"
#include "Input_Output_Declarations.h"

class TapRush : public Game {
public:
  byte lastPressedButtonId = 255;
  byte currentLedButtonPairId = 255;

  // Statistics
  byte totalInputCount = 0;
  byte totalCorrectCount = 0;

  // Game state
  unsigned long gameStartTime = 0;
  const int gameDuration = 15000;

  TapRush() {
    name = "TapRush";
  }

  void runGame() override {
    while (true){
      initializeGame();

      lcd.clear();
      centerTextLCDOutput("-- Tap Rush --", 0);
      centerTextLCDOutput("Tap To Start", 2);
      waitForUserInputToContinue();
      countdownLCDOutput(2);
      centerTextLCDOutput("! GOOO !", 2);


      lightUpNextRandomLED();
      gameStartTime = millis();

      while (millis() - gameStartTime <= gameDuration) {
        checkDebouncedButtonPress(0);
        checkDebouncedButtonPress(1);
        checkDebouncedButtonPress(2);
        checkDebouncedButtonPress(3);
        checkDebouncedButtonPress(4);
        checkDebouncedButtonPress(5);
      }
      turnOffLed(currentLedButtonPairId);

      endOfGameLCDOutput();

      playGameOver();

      flashAllLed(3, 300);

      delay(2000);

      waitForUserInputToContinue();  
    }
  }

private: 
  bool checkDebouncedButtonPress(byte buttonId) {
    if (digitalRead(ledButtonPairs[buttonId].buttonPin) == LOW && lastPressedButtonId != buttonId) {
        handleButtonPress(buttonId);
        return true;
    }
    return false;
  }   

  void handleButtonPress(byte buttonId){
    lastPressedButtonId = buttonId;
    Serial.print("Button ");
    Serial.print(buttonId);
    Serial.println(" was pressed");
    Serial.println(buttonId);
    totalInputCount += 1;
    if (lastPressedButtonId == currentLedButtonPairId) {
      totalCorrectCount += 1;
      lightUpNextRandomLED();
    }
  }

  void initializeGame(){
    lastPressedButtonId = 255;
    currentLedButtonPairId = 255;
    totalInputCount = 0;
    totalCorrectCount = 0;
    gameStartTime = 0;
  }

  void lightUpNextRandomLED(){
   if (currentLedButtonPairId != 255) {
    turnOffLed(currentLedButtonPairId);
  }
  currentLedButtonPairId = getDifferentRandomLedButtonPairId(lastPressedButtonId);
  turnOnLed(currentLedButtonPairId);
  Serial.print("Tap LED ");
  Serial.println(currentLedButtonPairId);
  }

  void endOfGameLCDOutput(){
    int averageTime;
    if (totalCorrectCount == 0) {
      averageTime = gameDuration;
    } else {
      averageTime = gameDuration/totalCorrectCount;
      Serial.print(totalCorrectCount);
    }
    int accuracy;
    if (totalCorrectCount == 0) {
      accuracy = 100;
    } else {
      accuracy = 100 * totalCorrectCount / totalInputCount;
    }
    lcd.clear();
    centerTextLCDOutput("Game Over!", 0);
    centerTextLCDOutput("Avg: " + String(averageTime) + "ms", 1);
    centerTextLCDOutput("Correct: " + String(totalCorrectCount), 2);
    centerTextLCDOutput("Accuracy: " + String(accuracy) + "%", 3);
  }
};