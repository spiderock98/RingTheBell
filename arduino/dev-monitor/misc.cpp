#include "misc.h"

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {7, 8, 9, 10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {3, 4, 5, 6};  //connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

extern bool flagRepeatSetting, flagCusSetting, flagEnRelay1, flagEnRelay2, flagEnRelay3, flagSetRTC, flagSetRfDuration;
extern byte iCusEvents;
extern volatile int16_t iCusAddressEEProm;
extern uint8_t compareDuration1, compareDuration2, compareDuration3;
extern uint8_t lastDuration1, lastDuration2, lastDuration3;

// LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2);
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // newer LCD

void WelcomeInterface()
{
    uint8_t bell[8] = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
    uint8_t clock[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
    uint8_t u71[8] = {0x04, 0x09, 0x03, 0x12, 0x12, 0x12, 0x0C, 0x00};
    uint8_t e1[8] = {0x08, 0x10, 0x0E, 0x11, 0x1F, 0x10, 0x0E, 0x00};
    // uint8_t e6[8] = {0x04, 0x0A, 0x00, 0x11, 0x1F, 0x10, 0x0E, 0x00};
    // uint8_t u75[8] = {0x00, 0x01, 0x13, 0x12, 0x12, 0x0C, 0x00, 0x0C};
    // uint8_t e65[8] = {0x04, 0x0A, 0x00, 0x11, 0x1F, 0x10, 0x0E, 0x04};
    uint8_t check[8] = {0x00, 0x00, 0x01, 0x03, 0x16, 0x1C, 0x08, 0x00};
    // uint8_t u3[8] = {0x0C, 0x04, 0x00, 0x12, 0x12, 0x12, 0x0C, 0x00};
    uint8_t cancel[8] = {0x00, 0x11, 0x1B, 0x0E, 0x0E, 0x1B, 0x11, 0x00};
    uint8_t temp[8] = {B11000, B11000, B00110, B01001, B01000, B01000, B01001, B00110};
    // uint8_t trash[8] = {B01110, B11111, B00000, B11111, B11111, B11111, B11111, B01110};

    lcd.init();
    lcd.backlight();

    lcd.createChar(0, bell);
    lcd.createChar(1, clock);
    lcd.createChar(2, u71);
    lcd.createChar(3, e1);
    lcd.createChar(4, check);
    lcd.createChar(5, cancel);
    lcd.createChar(6, temp);

    lcd.setCursor(0, 0);
    lcd.print("AUTO CHURCH BELL");
    lcd.setCursor(3, 1);
    for (byte i = 0; 5 - i;)
    {
        lcd.printByte(0);
        delay(200);
        lcd.printByte(1);
        delay(100);
        ++i;
    }
    lcd.clear();
}

void setRfDuration()
{
    flagSetRfDuration = true;
    char charVal;
    byte decVal;
    uint8_t arrInfo[3];

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("A: mm ph B: mm ph");
    lcd.setCursor(0, 1);
    lcd.print("    C: mm ph    ");
    lcd.blink();
    lcd.cursor();

lbRelay1Min:
    // hang chuc
    lcd.setCursor(3, 0);
    charVal = keypad.waitForKey(); // blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
        lcd.print(charVal);
    else if (charVal == 'B')
        goto lbRelay2Min;
    else
        goto lbRelay1Min;

    // hang don vi
    charVal = keypad.waitForKey(); // blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if ((decVal >= 60) || (decVal == 0))
            goto lbRelay1Min;
        lcd.print(charVal);
        arrInfo[0] = decVal;
    }
    else
        goto lbRelay1Min;

lbRelay2Min:
    // hang chuc
    lcd.setCursor(12, 0);
    charVal = keypad.waitForKey(); // blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
        lcd.print(charVal);
    else if (charVal == 'B')
        goto lbRelay3Min;
    else if (charVal == 'C')
        goto lbRelay1Min;
    else
        goto lbRelay2Min;

    // hang don vi
    charVal = keypad.waitForKey(); // blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if ((decVal >= 60) || (decVal == 0))
            goto lbRelay2Min;
        lcd.print(charVal);
        arrInfo[1] = decVal;
    }
    else
        goto lbRelay2Min;

lbRelay3Min:
    // hang chuc
    lcd.setCursor(7, 1);
    charVal = keypad.waitForKey(); // blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
        lcd.print(charVal);
    else if (charVal == 'C')
        goto lbRelay2Min;
    else
        goto lbRelay3Min;

    // hang don vi
    charVal = keypad.waitForKey(); // blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if ((decVal >= 60) || (decVal == 0))
            goto lbRelay3Min;
        lcd.print(charVal);
        arrInfo[2] = decVal;
    }
    else
        goto lbRelay3Min;

    delay(1000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Xac Nhan ?   ");

    lcd.setCursor(0, 1);
    lcd.printByte(4); //checked symbol
    lcd.print(" A:OK");
    lcd.setCursor(9, 1);
    lcd.printByte(5); // cancel symbol
    lcd.setCursor(10, 1);
    lcd.print(" D:Huy");

lbConfirm:
    charVal = keypad.waitForKey(); //blocking
    if (charVal == 'D')
        ;
    else if (charVal == 'A')
    {
        compareDuration1 = arrInfo[0];
        compareDuration2 = arrInfo[1];
        compareDuration3 = arrInfo[2];
        // save to eeprom
        EEPROM.write(1025, compareDuration1);
        delay(4);
        EEPROM.write(1026, compareDuration2);
        delay(4);
        EEPROM.write(1027, compareDuration3);
        delay(4);
    }
    else
        goto lbConfirm;

    lcd.clear();
    flagSetRfDuration = false;
}

void setRTC()
{
    flagSetRTC = true;
    char charVal;
    byte decVal, arrRTC[8];

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Th");
    lcd.printByte(2);
    lcd.setCursor(6, 0);
    lcd.print("dd-mm-yyyy");
    lcd.setCursor(4, 1);
    lcd.print("hh-mm-ss");
    lcd.blink();
    lcd.cursor();

// 1:Sun , 2:Mon, ...
lbWday:
    lcd.setCursor(4, 0);
    charVal = keypad.waitForKey(); // blocking
    decVal = char2byte(charVal);
    if ((!isSpecialChar(charVal)) && (decVal <= 7) && (decVal != 0))
    {
        if (charVal == '1')
            lcd.print("CN");
        else
            lcd.print(charVal);
        arrRTC[0] = decVal; // set to ds3020 RTC
    }
    else if (charVal == 'B')
        goto lbMday;
    else
        goto lbWday;

lbMday:
    // hang chuc
    lcd.setCursor(6, 0);
    charVal = keypad.waitForKey(); //blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
        lcd.print(charVal);
    else if (charVal == 'B')
        goto lbMonth;
    else if (charVal == 'C')
        goto lbWday;
    else
        goto lbMday;

    // hang don vi
    charVal = keypad.waitForKey(); //blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if ((decVal >= 32) || (decVal == 0))
            goto lbMday;
        lcd.print(charVal);
        arrRTC[1] = decVal;
    }
    else
        goto lbMday;

lbMonth:
    // hang chuc
    lcd.setCursor(9, 0);
    charVal = keypad.waitForKey(); //blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
        lcd.print(charVal);
    else if (charVal == 'B')
        goto lbYear;
    else if (charVal == 'C')
        goto lbMday;
    else
        goto lbMonth;

    // hang don vi
    charVal = keypad.waitForKey(); //blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if ((decVal >= 13) || (decVal == 0))
            goto lbMonth;
        lcd.print(charVal);
        arrRTC[2] = decVal;
    }
    else
        goto lbMonth;

lbYear:
    // hang nghin
    lcd.setCursor(12, 0);
    charVal = keypad.waitForKey(); // blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
        lcd.print(charVal);
    else if (charVal == 'B')
        goto lbHour;
    else if (charVal == 'C')
        goto lbMonth;
    else
        goto lbYear;

    // hang tram
    charVal = keypad.waitForKey(); // blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if (decVal <= 19)
            goto lbYear;
        lcd.print(charVal);
        arrRTC[3] = decVal;
    }
    else
        goto lbYear;

    // hang chuc
    charVal = keypad.waitForKey(); // blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
        lcd.print(charVal);
    else
        goto lbYear;

    // hang don vi
    charVal = keypad.waitForKey(); // blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        lcd.print(charVal);
        arrRTC[4] = decVal;
    }
    else
        goto lbYear;

lbHour:
    // hang chuc
    lcd.setCursor(4, 1);
    charVal = keypad.waitForKey(); //blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
        lcd.print(charVal);
    else if (charVal == 'B')
        goto lbMinute;
    else if (charVal == 'C')
        goto lbYear;
    else
        goto lbHour;

    // hang don vi
    charVal = keypad.waitForKey(); // blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if (decVal >= 24)
            goto lbHour;
        lcd.print(charVal);
        arrRTC[5] = decVal;
    }
    else
        goto lbHour;

lbMinute:
    // hang chuc
    lcd.setCursor(7, 1);
    charVal = keypad.waitForKey(); // blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
        lcd.print(charVal);
    else if (charVal == 'B')
        goto lbSecond;
    else if (charVal == 'C')
        goto lbHour;
    else
        goto lbMinute;

    // hang don vi
    charVal = keypad.waitForKey(); // blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if (decVal >= 60)
            goto lbMinute;
        lcd.print(charVal);
        arrRTC[6] = decVal;
    }
    else
        goto lbMinute;

lbSecond:
    // hang chuc
    lcd.setCursor(10, 1);
    charVal = keypad.waitForKey(); // blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
        lcd.print(charVal);
    else if (charVal == 'C')
        goto lbMinute;
    else
        goto lbSecond;

    // hang don vi
    charVal = keypad.waitForKey(); // blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if (decVal >= 60)
            goto lbSecond;
        lcd.print(charVal);
        arrRTC[7] = decVal;
    }
    else
        goto lbSecond;

    delay(1000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cap nhat tgian ?");

    lcd.setCursor(0, 1);
    lcd.printByte(4); // checked symbol
    lcd.print(" A:OK");
    lcd.setCursor(9, 1);
    lcd.printByte(5); // cancel symbol
    lcd.setCursor(10, 1);
    lcd.print(" D:Huy");

lbConfirm:
    charVal = keypad.waitForKey(); // blocking
    if (charVal == 'D')
        ;
    else if (charVal == 'A')
    {
        t.wday = arrRTC[0];
        t.mday = arrRTC[1];
        t.mon = arrRTC[2];
        t.year = arrRTC[3] * 100 + arrRTC[4];
        t.hour = arrRTC[5];
        t.min = arrRTC[6];
        t.sec = arrRTC[7];

        DS3231_set(t);
    }
    else
        goto lbConfirm;

    flagSetRTC = false;
    // return home screen
    lcd.clear();
}

// Home screen UI
void lcdHomeScreen()
{
    lcd.noBlink();
    lcd.noCursor();
    lcd.setCursor(0, 0);
    lcd.print("Th");
    lcd.printByte(2);
    lcd.print(" ");
    if (t.wday == 1)
        lcd.print("CN");
    else
        lcd.print(t.wday);
    lcd.print(" ");
    lcd.print(t.mday);
    lcd.print("-");
    lcd.print(t.mon);
    lcd.print("-");
    lcd.print(t.year);

    lcd.setCursor(0, 1);
    lcd.print(t.hour);
    lcd.print(":");
    lcd.print(t.min);
    lcd.print(":");
    lcd.print(t.sec);
    lcd.print("  ");
    lcd.setCursor(10, 1);
    lcd.print(DS3231_get_treg());
    lcd.printByte(6);
}

// 1 phút check hàm này 1 lần
void alarm()
{
    uint8_t currentWeekDay = t.wday;
    uint8_t currentSecond = t.sec;
    uint8_t currentMinute = t.min;
    uint8_t currentHour = t.hour;
    uint8_t currentMonthDay = t.mday;
    uint8_t currentMonth = t.mon;
    int16_t currentYear = t.year;

    // alarm sự kiện lặp lại
    switch (currentWeekDay)
    {
    case 2: // Mon
        for (int addr = 0; 28 - addr;)
        {
            if (currentHour == EEPROM.read(addr + 1))
            {
                if (currentMinute == EEPROM.read(addr + 2))
                {
                    if (EEPROM.read(addr + 3)) // relay 1
                    {
                        flagEnRelay1 = true;
                        digitalWrite(OUT1, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration1 = t.min;
                        compareDuration1 = EEPROM.read(addr + 6); // update duration
                    }
                    if (EEPROM.read(addr + 4)) // relay 2
                    {
                        flagEnRelay2 = true;
                        digitalWrite(OUT2, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration2 = t.min;
                        compareDuration2 = EEPROM.read(addr + 6); // update duration
                    }
                    if (EEPROM.read(addr + 5)) // relay 3
                    {
                        flagEnRelay3 = true;
                        digitalWrite(OUT3, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration3 = t.min;
                        compareDuration3 = EEPROM.read(addr + 6); // update duration
                    }
                    return;
                }
            }
            addr += 7; // Sáng -> Trưa -> Chiều -> Tối
        }
        break;
    case 3: // Tue
        for (int addr = 28; 56 - addr;)
        {
            if (currentHour == EEPROM.read(addr + 1))
            {
                if (currentMinute == EEPROM.read(addr + 2))
                {
                    if (EEPROM.read(addr + 3)) // relay 1
                    {
                        flagEnRelay1 = true;
                        digitalWrite(OUT1, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration1 = t.min;
                        compareDuration1 = EEPROM.read(addr + 6); // update duration
                    }
                    if (EEPROM.read(addr + 4)) // relay 2
                    {
                        flagEnRelay2 = true;
                        digitalWrite(OUT2, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration2 = t.min;
                        compareDuration2 = EEPROM.read(addr + 6); // update duration
                    }
                    if (EEPROM.read(addr + 5)) // relay 3
                    {
                        flagEnRelay3 = true;
                        digitalWrite(OUT3, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration3 = t.min;
                        compareDuration3 = EEPROM.read(addr + 6); // update duration
                    }
                    return;
                }
            }
            addr += 7;
        }
        break;
    case 4: // Wed
        for (int addr = 56; 84 - addr;)
        {
            if (currentHour == EEPROM.read(addr + 1))
            {
                if (currentMinute == EEPROM.read(addr + 2))
                {
                    if (EEPROM.read(addr + 3)) // relay 1
                    {
                        flagEnRelay1 = true;
                        digitalWrite(OUT1, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration1 = t.min;
                        compareDuration1 = EEPROM.read(addr + 6); // update duration
                    }
                    if (EEPROM.read(addr + 4)) // relay 2
                    {
                        flagEnRelay2 = true;
                        digitalWrite(OUT2, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration2 = t.min;
                        compareDuration2 = EEPROM.read(addr + 6); // update duration
                    }
                    if (EEPROM.read(addr + 5)) // relay 3
                    {
                        flagEnRelay3 = true;
                        digitalWrite(OUT3, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration3 = t.min;
                        compareDuration3 = EEPROM.read(addr + 6); // update duration
                    }
                    return;
                }
            }
            addr += 7;
        }
        break;
    case 5: // Thus
        for (int addr = 84; 112 - addr;)
        {
            if (currentHour == EEPROM.read(addr + 1))
            {
                if (currentMinute == EEPROM.read(addr + 2))
                {
                    if (EEPROM.read(addr + 3)) // relay 1
                    {
                        flagEnRelay1 = true;
                        digitalWrite(OUT1, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration1 = t.min;
                        compareDuration1 = EEPROM.read(addr + 6); // update duration
                    }
                    if (EEPROM.read(addr + 4)) // relay 2
                    {
                        flagEnRelay2 = true;
                        digitalWrite(OUT2, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration2 = t.min;
                        compareDuration2 = EEPROM.read(addr + 6); // update duration
                    }
                    if (EEPROM.read(addr + 5)) // relay 3
                    {
                        flagEnRelay3 = true;
                        digitalWrite(OUT3, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration3 = t.min;
                        compareDuration3 = EEPROM.read(addr + 6); // update duration
                    }
                    return;
                }
            }
            addr += 7;
        }
        break;
    case 6: // Fri
        for (int addr = 112; 140 - addr;)
        {
            if (currentHour == EEPROM.read(addr + 1))
            {
                if (currentMinute == EEPROM.read(addr + 2))
                {
                    if (EEPROM.read(addr + 3)) // relay 1
                    {
                        flagEnRelay1 = true;
                        digitalWrite(OUT1, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration1 = t.min;
                        compareDuration1 = EEPROM.read(addr + 6); // update duration
                    }
                    if (EEPROM.read(addr + 4)) // relay 2
                    {
                        flagEnRelay2 = true;
                        digitalWrite(OUT2, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration2 = t.min;
                        compareDuration2 = EEPROM.read(addr + 6); // update duration
                    }
                    if (EEPROM.read(addr + 5)) // relay 3
                    {
                        flagEnRelay3 = true;
                        digitalWrite(OUT3, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration3 = t.min;
                        compareDuration3 = EEPROM.read(addr + 6); // update duration
                    }
                    return;
                }
            }
            addr += 7;
        }
        break;
    case 7: // Sat
        for (int addr = 140; 168 - addr;)
        {
            if (currentHour == EEPROM.read(addr + 1))
            {
                if (currentMinute == EEPROM.read(addr + 2))
                {
                    if (EEPROM.read(addr + 3)) // relay 1
                    {
                        flagEnRelay1 = true;
                        digitalWrite(OUT1, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration1 = t.min;
                        compareDuration1 = EEPROM.read(addr + 6); // update duration
                    }
                    if (EEPROM.read(addr + 4)) // relay 2
                    {
                        flagEnRelay2 = true;
                        digitalWrite(OUT2, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration2 = t.min;
                        compareDuration2 = EEPROM.read(addr + 6); // update duration
                    }
                    if (EEPROM.read(addr + 5)) // relay 3
                    {
                        flagEnRelay3 = true;
                        digitalWrite(OUT3, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration3 = t.min;
                        compareDuration3 = EEPROM.read(addr + 6); // update duration
                    }
                    return;
                }
            }
            addr += 7;
        }
        break;
    case 1: // Sun
        for (int addr = 168; 196 - addr;)
        {
            if (currentHour == EEPROM.read(addr + 1))
            {
                if (currentMinute == EEPROM.read(addr + 2))
                {
                    if (EEPROM.read(addr + 3)) // relay 1
                    {
                        flagEnRelay1 = true;
                        digitalWrite(OUT1, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration1 = t.min;
                        compareDuration1 = EEPROM.read(addr + 6); // update duration
                    }
                    if (EEPROM.read(addr + 4)) // relay 2
                    {
                        flagEnRelay2 = true;
                        digitalWrite(OUT2, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration2 = t.min;
                        compareDuration2 = EEPROM.read(addr + 6); // update duration
                    }
                    if (EEPROM.read(addr + 5)) // relay 3
                    {
                        flagEnRelay3 = true;
                        digitalWrite(OUT3, 1);
                        digitalWrite(OUT5, 1); // relay ON mass wire
                        lastDuration3 = t.min;
                        compareDuration3 = EEPROM.read(addr + 6); // update duration
                    }
                    return;
                }
            }
            addr += 7;
        }
        break;

    default:
        lcd.clear();
        lcd.print("Error");
        break;
    }

    // alarm sự kiện 1 lần
    int iCusAddr = 187;
    for (int i = 0; EEPROM.read(196) - i;)
    { // loop num of length times
        do
        { // pass null event to generate iCusAddr
            iCusAddr += 10;
        } while ((EEPROM.read(iCusAddr) == 0) && (iCusAddr < 1024));

        if (currentMinute == EEPROM.read(iCusAddr + 5))
            if (currentHour == EEPROM.read(iCusAddr + 4))
                if (currentMonthDay == EEPROM.read(iCusAddr))
                    if (currentMonth == EEPROM.read(iCusAddr + 1))
                        if ((currentYear / 100) == EEPROM.read(iCusAddr + 2))
                            if ((currentYear % 100) == EEPROM.read(iCusAddr + 3)) // an event is match
                            {
                                // check bit if enable relay or not
                                if (EEPROM.read(iCusAddr + 6))
                                {
                                    flagEnRelay1 = true;
                                    digitalWrite(OUT1, 1);
                                    digitalWrite(OUT5, 1); // relay ON mass wire
                                    lastDuration1 = t.min;
                                    compareDuration1 = EEPROM.read(iCusAddr + 9); // update duration
                                }
                                if (EEPROM.read(iCusAddr + 7))
                                {
                                    flagEnRelay2 = true;
                                    digitalWrite(OUT2, 1);
                                    digitalWrite(OUT5, 1); // relay ON mass wire
                                    lastDuration2 = t.min;
                                    compareDuration2 = EEPROM.read(iCusAddr + 9); // update duration
                                }
                                if (EEPROM.read(iCusAddr + 8))
                                {
                                    flagEnRelay3 = true;
                                    digitalWrite(OUT3, 1);
                                    digitalWrite(OUT5, 1); // relay ON mass wire
                                    lastDuration3 = t.min;
                                    compareDuration3 = EEPROM.read(iCusAddr + 9); // update duration
                                }

                                // delete this match events
                                EEPROM.write(iCusAddr, 0);
                                delay(4);
                                EEPROM.write(196, EEPROM.read(196) - 1);
                                delay(4); // An EEPROM write takes 3.3 ms to complete
                                iCusEvents = EEPROM.read(196);
                                iCusAddressEEProm = 187;

                                return; // exit right on match event every 1 minutes
                            }
        ++i;
    }
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
