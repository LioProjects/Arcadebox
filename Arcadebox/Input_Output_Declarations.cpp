#include "Arduino.h"
#include "Input_Output_Declarations.h"


// Define the LCD object
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Define custom characters
uint8_t filledRect[8] = {
    0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F
};

uint8_t average[8] = {
    0x04, 0x04, 0x0E, 0x15, 0x15, 0x0E, 0x04, 0x04
};

uint8_t milliSecond[8] = {
    0x1B, 0x15, 0x11, 0x17, 0x14, 0x07, 0x01, 0x07
};

uint8_t heart[8] = {
    0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00
};

const byte randomSeedPin = A7;

// Pin mappings
const byte BUZZER_PIN = A0;

const byte ledPin0 = 5;
const byte ledPin1 = 3;
const byte ledPin2 = A1;
const byte ledPin3 = 9;
const byte ledPin4 = 11;
const byte ledPin5 = 6;

const byte buttonPin0 = 4;
const byte buttonPin1 = 2;
const byte buttonPin2 = A2;
const byte buttonPin3 = 8;
const byte buttonPin4 = 12;
const byte buttonPin5 = 7;
//Todo: change values to actual notes for new library
// Define the LED and button pairs
const LedButtonPair ledButtonPairs[] = {
    {0, ledPin0, buttonPin0, 700},
    {1, ledPin1, buttonPin1, 800},
    {2, ledPin2, buttonPin2, 900},
    {3, ledPin3, buttonPin3, 1000},
    {4, ledPin4, buttonPin4, 1100},
    {5, ledPin5, buttonPin5, 1200},
};

const byte navigateLeftButtonId = 3;
const byte navigateRightButtonId = 5;
const byte selectButtonId = 4;

const char *wrongInput = "wrong:d=8,o=5,b=160:e4,c4";
const char *correctInput = "correct:d=8,o=5,b=160:e4,b4";
const char *levelUp = "levelup:d=8,o=5,b=190:e4,a4,b4,e5";
const char *gameOver = "gameover:d=8,o=5,b=190:b4,g4,e4,b4";

void initializeLedButtonPair(){
  for (byte i = 0; i < sizeof(ledButtonPairs)/sizeof(ledButtonPairs[0]); i++){
    pinMode(ledButtonPairs[i].ledPin, OUTPUT);
    pinMode(ledButtonPairs[i].buttonPin, INPUT_PULLUP);
  }
}

// Initialize the LCD and custom characters
void initializeLCD() {
    lcd.init();
    lcd.backlight();
    lcd.createChar(0, filledRect);
    lcd.createChar(1, average);
    lcd.createChar(2, milliSecond);
    lcd.createChar(3, heart);
}

void centerTextLCDOutput(String text, byte row) {
  clearLineLCDOutput(row);
  lcd.setCursor((20 - text.length())/2, row);
  lcd.print(text);
}


void clearLineLCDOutput(byte row) {
  lcd.setCursor(0, row);
  lcd.print("                    ");
}

void countdownLCDOutput(byte row) {
  centerTextLCDOutput("3", row);
  delay(1000);
  centerTextLCDOutput("2", row);
  delay(1000);
  centerTextLCDOutput("1", row);
  delay(1000);
  clearLineLCDOutput(row);
}

void waitForUserInputToContinue() {
  turnOnLed(ledButtonPairs[selectButtonId].id);
  while (true) {
    if (!digitalRead(ledButtonPairs[selectButtonId].buttonPin)) {
      Serial.println("button p43wss");
      break;
    }
  }
  turnOffLed(ledButtonPairs[selectButtonId].id);
}

byte getDifferentRandomLedButtonPairId(byte previousLedButtonPairId){
  int newId;
  do {
    newId = random(0, sizeof(ledButtonPairs)/sizeof(ledButtonPairs[0]));
  } while (newId == previousLedButtonPairId);
  return newId;
}

void turnOnLed(byte ledId) {
  digitalWrite(ledButtonPairs[ledId].ledPin, HIGH);
}

void turnOffLed(byte ledId) {
  digitalWrite(ledButtonPairs[ledId].ledPin, LOW);
}

void flashLed(byte ledId, byte amount, int spacing){
  for (byte i = 0; i < amount; i ++){
    turnOnLed(ledId);
    delay(spacing);
    turnOffLed(ledId);
    delay(spacing);
  }
}

void flashAllLed(byte amount, int spacing){
  for (byte i = 0; i < amount; i ++){
    for (byte i = 0; i < sizeof(ledButtonPairs)/sizeof(ledButtonPairs[0]); i++){
      turnOnLed(i);
    }
    delay(spacing);
    for (byte i = 0; i < sizeof(ledButtonPairs)/sizeof(ledButtonPairs[0]); i++){
      turnOffLed(i);
    }
    delay(spacing);
  }
}

void playWrongInput() {
  anyrtttl::blocking::play(BUZZER_PIN, wrongInput);
}

void playCorrectInput() {
  anyrtttl::blocking::play(BUZZER_PIN, correctInput);
}

void playLevelUp() {
  anyrtttl::blocking::play(BUZZER_PIN, levelUp);
}

void playGameOver(){
  anyrtttl::blocking::play(BUZZER_PIN, gameOver);
}

