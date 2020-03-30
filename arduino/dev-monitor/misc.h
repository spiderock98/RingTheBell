#pragma once

#include <Keypad.h>
#include <Wire.h>
#include <ds3231.h>
#include <LiquidCrystal_I2C.h>

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args) write(args);
#else
#define printByte(args) print(args, BYTE);
#endif

extern LiquidCrystal_I2C lcd; // set the LCD address to 0x27 for a 16 chars and 2 line display
extern Keypad keypad;

void lcdDefaultInterface();
bool isSpecialChar(char key);
byte char2byte(char key);
