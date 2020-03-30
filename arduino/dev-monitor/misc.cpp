#include "misc.h"

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {6, 7, 8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 3, 4, 5}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2);
void lcdDefaultInterface()
{
    uint8_t bell[8] = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
    uint8_t clock[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
    uint8_t u71[8] = {0x04, 0x09, 0x03, 0x12, 0x12, 0x12, 0x0C, 0x00};
    uint8_t e1[8] = {0x08, 0x10, 0x0E, 0x11, 0x1F, 0x10, 0x0E, 0x00};
    uint8_t e6[8] = {0x04, 0x0A, 0x00, 0x11, 0x1F, 0x10, 0x0E, 0x00};
    uint8_t u75[8] = {0x00, 0x01, 0x13, 0x12, 0x12, 0x0C, 0x00, 0x0C};
    uint8_t e65[8] = {0x04, 0x0A, 0x00, 0x11, 0x1F, 0x10, 0x0E, 0x04};
    uint8_t check[8] = {0x00, 0x00, 0x01, 0x03, 0x16, 0x1C, 0x08, 0x00};
    uint8_t u3[8] = {0x0C, 0x04, 0x00, 0x12, 0x12, 0x12, 0x0C, 0x00};
    uint8_t cancel[8] = {0x00, 0x11, 0x1B, 0x0E, 0x0E, 0x1B, 0x11, 0x00};

    lcd.init();
    lcd.backlight();

    lcd.createChar(0, bell);
    lcd.createChar(1, clock);
    lcd.createChar(2, u71);
    lcd.createChar(3, e1);
    // lcd.createChar(4, e6);
    // lcd.createChar(5, u75);
    // lcd.createChar(6, e65);
    lcd.createChar(4, check);
    lcd.createChar(5, cancel);

    lcd.setCursor(0, 0);
    lcd.print("AUTO CHURCH BELL");
    lcd.setCursor(3, 1);
    for (byte i = 0; 5 - i;)
    {
        lcd.printByte(0);
        delay(200);
        lcd.printByte(1);
        delay(200);
        ++i;
    }
    // lcd.noBacklight();
    lcd.clear();
}
bool isSpecialChar(char key)
{
    if ((key == '*') || (key == '#') || (key == 'A') || (key == 'B') || (key == 'C') || (key == 'D'))
        return true;
    else
        return false;
}
byte char2byte(char key)
{
    return (byte)key - 48;
}