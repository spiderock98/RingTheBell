#pragma once

#include <Keypad.h>
#include <Wire.h>
#include <ds3231.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

// pin confige
#define RF0 A3
#define RF1 A0
#define RF2 A1
#define RF3 A2
#define OUT1 11
#define OUT2 12
#define OUT3 13
#define OUT4 A6
#define defaultDuration 5

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args) write(args);
#else
#define printByte(args) print(args, BYTE);
#endif

extern LiquidCrystal_I2C lcd; // set the LCD address to 0x27 for a 16 chars and 2 line display
extern Keypad keypad;
extern struct ts t;

void WelcomeInterface();
void lcdHomeScreen();
void setRTC();
bool isSpecialChar(char key);
byte char2byte(char key);
void alarm();
void setRfTimer();