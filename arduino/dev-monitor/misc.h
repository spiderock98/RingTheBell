#pragma once

#include <Keypad.h>
#include <Wire.h>
#include <ds3231.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

#define RF0 A7
#define RF1 A3
#define RF2 A2
#define RF3 A6
// relay
#define OUT1 A1
#define OUT2 11
#define OUT3 A0
// pulse per hour
#define OUT4 12
#define OUT5 13
// #define defaultDuration 5

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
void setRfDuration();