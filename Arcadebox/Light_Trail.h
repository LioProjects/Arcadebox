#include "Game.h"
#include "Input_Output_Declarations.h"

class LightTrail : public Game {
public:

  byte level = 1;
  byte lifes = 3;
  byte lightSequence[50];

  int lightSequencePointer = 0;
  bool falseInput = 0;

  byte lastPressedButtonId;
  long lastLEDLightUpTime;
  byte currentLitLED;
  int actionDuration;

  LightTrail() {
    name = "LightTrail";
  }

  void runGame() override {
    while (true){
      initializeGame();
      
      lcd.clear();
      centerTextLCDOutput("-- LIGHT TRAIL --", 0);

      centerTextLCDOutput("Watch & Memorize", 1);
      centerTextLCDOutput("Then Repeat!", 2);
      centerTextLCDOutput("Tap to Start", 3);
      waitForUserInputToContinue();
      clearLineLCDOutput(1);
      clearLineLCDOutput(2);
      clearLineLCDOutput(3);
      
      while(lifes > 0){
        updateStatisticsLCDOutput();
        centerTextLCDOutput("MEMORIZE", 3);
        
        delay(1000);

        playLightSequence();
        centerTextLCDOutput("REPEAT", 3);

        while (lightSequencePointer < level && !falseInput){
          checkDebouncedButtonPress(0);
          checkDebouncedButtonPress(1);
          checkDebouncedButtonPress(2);
          checkDebouncedButtonPress(3);
          checkDebouncedButtonPress(4);
          checkDebouncedButtonPress(5);
          if (currentLitLED >= 0 && ((millis() - lastLEDLightUpTime) > actionDuration)){
            turnOffLed(currentLitLED);
          }
        }
        if (lightSequencePointer == level){
          handleLevelUp();
          centerTextLCDOutput("Level Up", 3);
        }
        else {
          centerTextLCDOutput("--! INEGLEIT !--", 3);
          handleFalseInput();
        }
        delay(1000);
      }
      centerTextLCDOutput("End of Game", 3);

      playGameOver();
      flashAllLed(3, 300);

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
    turnOffLed(lastPressedButtonId);
    turnOnLed(buttonId);
    lastPressedButtonId = buttonId;
    Serial.print("Button ");
    Serial.print(buttonId);
    Serial.println(" was pressed");
    Serial.println(buttonId);
    if((buttonId == lightSequence[lightSequencePointer])){
      lightSequencePointer += 1;
      //playGoodInput();
    }
    else{
      falseInput = true;
    }
  }

  void initializeGame(){
    level = 1;
    lifes = 3;

    lightSequencePointer = 0;
    falseInput = 0;

    lastPressedButtonId = 255;
    lastLEDLightUpTime = -1;
    currentLitLED = 255;
    actionDuration = 500;

    lightSequence[0] = getDifferentRandomLedButtonPairId(lastPressedButtonId);
  }


  void playLightSequence(){
    for (int i = 0; i < level; i++){
      Serial.print("lighing up: ");
      Serial.println(lightSequence[i]);
      turnOnLed(lightSequence[i]);
      delay(actionDuration);
      turnOffLed(lightSequence[i]);
      if (i < level -1){
        delay(actionDuration);
      }
    }
  }

  void updateStatisticsLCDOutput(){
    lcd.setCursor(0, 2);
    lcd.print("Level: ");
    lcd.print(level);

    lcd.setCursor(17, 2);
    for (byte i = 0; i < 3 - lifes; i++){
      lcd.print(" ");
    }
    for (byte i = 0; i < lifes; i++){
      lcd.printByte(3);
    }
  }
  

  void handleLevelUp(){
    level += 1;

    turnOffLed(ledButtonPairs[lastPressedButtonId].id);

    updateStatisticsLCDOutput();

    playLevelUp();
    
    if (actionDuration > 200) {
    actionDuration -= 50;
      if (actionDuration <= 200) {
          actionDuration = 200;
          centerTextLCDOutput("Max Speed Achieved", 3);
          delay(2000);
      }
    }

    lightSequence[level - 1] = getDifferentRandomLedButtonPairId(lastPressedButtonId);

    lightSequencePointer = 0;
    lastPressedButtonId = 255;

  }

  void handleFalseInput(){
    lifes -= 1;
    updateStatisticsLCDOutput();
    turnOffLed(ledButtonPairs[lastPressedButtonId].id);
    playWrongInput();
    
    flashLed(lightSequence[lightSequencePointer], 3, 300); 
    lightSequencePointer = 0;
    lastPressedButtonId = 255;
    falseInput = false;
  }
};