#include "Input_Output_Declarations.h"
#include "Menu_Entry.h"
#include "Tap_Rush.h"
#include "Light_Trail.h"

TapRush tapRushInstance;
LightTrail lightTrailInstance;

MenuEntry menuEntries[]{
  MenuEntry(&tapRushInstance),
  MenuEntry(&lightTrailInstance),
};

long lastButtonPressTime;
byte currentGameIdCursorSelection = 0;

long lastCursorBlinkTime;
int blinkDuration = 500;
bool blink = true;


void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(randomSeedPin));
  initializeLedButtonPair();
  initializeLCD();
  
  centerTextLCDOutput("Select a Game", 0);
  printGamesOnLCD();
}

void loop() {
  blinkController();
  controlLCDBlinkOutput();
  controlUserNavigationAndSelectionButtonOutput();

  checkButtonPress(navigateLeftButtonId);
  checkButtonPress(selectButtonId);
  checkButtonPress(navigateRightButtonId);
}

void checkButtonPress(byte buttonId){
    if((digitalRead(ledButtonPairs[buttonId].buttonPin) == LOW) && (millis() - lastButtonPressTime > 250)){
    lastButtonPressTime = millis();
    //Rewrite last Gamename covered by the blink
    lcd.setCursor(menuEntries[currentGameIdCursorSelection].coordinates.col, menuEntries[currentGameIdCursorSelection].coordinates.row);
    lcd.print(menuEntries[currentGameIdCursorSelection].gameName);
    //Reset Cursor Blink
    lastCursorBlinkTime = millis() - blinkDuration;
    blink = false;
    Serial.println(buttonId);

    if (buttonId == navigateLeftButtonId){
      handleLeftButtonPress();
    }
    if (buttonId == selectButtonId){
      turnOffLed(navigateLeftButtonId);
      turnOffLed(selectButtonId);
      turnOffLed(navigateRightButtonId);
      handleSelectButtonPress();
    }
    if (buttonId == navigateRightButtonId){
      handleRightButtonPress();
    }
  }
}

void handleLeftButtonPress(){
  if(currentGameIdCursorSelection > 0){
    currentGameIdCursorSelection -= 1;
  }
}

void handleSelectButtonPress(){
  menuEntries[currentGameIdCursorSelection].game->runGame();
}

void handleRightButtonPress(){
  if(currentGameIdCursorSelection < (sizeof(menuEntries)/sizeof(menuEntries[0])-1)){
    currentGameIdCursorSelection += 1;
  }
}

void controlUserNavigationAndSelectionButtonOutput(){
  if (blink){
    turnOnLed(selectButtonId);
  }
  else {
    turnOffLed(selectButtonId);
  }
  if (currentGameIdCursorSelection == 0){
    turnOffLed(navigateLeftButtonId);
    turnOnLed(navigateRightButtonId);
  }
  else if (currentGameIdCursorSelection == sizeof(menuEntries)/sizeof(menuEntries[0]) -1 ){
    turnOffLed(navigateRightButtonId);
    turnOnLed(navigateLeftButtonId);
  }
  else {
    turnOnLed(navigateRightButtonId);
    turnOnLed(navigateLeftButtonId);
  }
}

void controlLCDBlinkOutput(){
  lcd.setCursor(menuEntries[currentGameIdCursorSelection].coordinates.col, menuEntries[currentGameIdCursorSelection].coordinates.row);
  if (blink){
    for (int i = 0; i < menuEntries[currentGameIdCursorSelection].gameName.length(); i++){
        lcd.printByte(0);
    }
  }
  else {
    lcd.print(menuEntries[currentGameIdCursorSelection].gameName);
  }
}


void blinkController(){
  while (millis() - lastCursorBlinkTime > blinkDuration){
    lastCursorBlinkTime = millis();
    if (blink){     
      blink = false;
    }
    else {
      blink = true;
    }
  }
}

void printGamesOnLCD(){
  for (byte i = 0; i < sizeof(menuEntries)/sizeof(menuEntries[0]); i++){
    lcd.setCursor(menuEntries[i].coordinates.col, menuEntries[i].coordinates.row);
    lcd.print(menuEntries[i].gameName);
  }
}

