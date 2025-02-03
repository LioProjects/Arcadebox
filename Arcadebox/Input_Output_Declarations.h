#ifndef Input_Output_Declarations_H
#define Input_Output_Declarations_H

#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <anyrtttl.h>

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

// Declare the LCD object
extern LiquidCrystal_I2C lcd;

//extern Tone tonePin;

// Declare custom characters
extern uint8_t filledRect[8];
extern uint8_t average[8];
extern uint8_t milliSecond[8];
extern uint8_t heart[8];

// Pin mappings
extern const byte randomSeedPin;
extern const byte BUZZER_PIN;
extern const byte ledPin0,    ledPin1,    ledPin2,    ledPin3,    ledPin4,    ledPin5;
extern const byte buttonPin0, buttonPin1, buttonPin2, buttonPin3, buttonPin4, buttonPin5;

extern const byte navigateLeftButtonId;
extern const byte navigateRightButtonId;
extern const byte selectButtonId;

// LED and button pairs
struct LedButtonPair {
    byte id;
    byte ledPin;
    byte buttonPin;
    int buzzFrequency;
};

extern const LedButtonPair ledButtonPairs[];

extern const char *wrongInput;
extern const char *correctInput;
extern const char *levelUp;

void initializeLedButtonPair();

void initializeLCD();

void centerTextLCDOutput(String text, byte row);

void listTextLCDOutput(String text[], byte row);

void clearLineLCDOutput(byte row);

void countdownLCDOutput(byte row);

void waitForUserInputToContinue();

byte getDifferentRandomLedButtonPairId(byte previousLedButtonPairId);

void turnOnLed(byte ledId);

void turnOffLed(byte ledId);

void flashLed(byte ledId, byte amount, int spacing);

void flashAllLed(byte amount, int spacing);

// Functions for different events
void playWrongInput();

void playCorrectInput();

void playLevelUp();

void playGameOver();

#endif