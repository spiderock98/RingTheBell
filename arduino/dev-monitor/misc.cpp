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

extern bool flagRepeatSetting, flagCusSetting, flagEnRelay1, flagEnRelay2, flagEnRelay3, flagSetRTC, flagSetRfTimer;
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

void setRfTimer()
{
    flagSetRfTimer = true;
    char charVal;
    byte decVal;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dung remote BAT");
    lcd.setCursor(0, 1);
    lcd.print(" TAT sau: mm ph ");
    lcd.blink();
    lcd.cursor();

lbMinute:
    // hang chuc
    lcd.setCursor(10, 1);
    charVal = keypad.waitForKey(); // blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
        lcd.print(charVal);
    else
        goto lbMinute;

    // hang don vi
    charVal = keypad.waitForKey(); // blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if ((decVal >= 60) || (decVal == 0))
            goto lbMinute;
        lcd.print(charVal);
    }
    else
        goto lbMinute;

    compareDuration1 = compareDuration2 = compareDuration3 = decVal;

    delay(1000);
    flagSetRfTimer = false;
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
        for (int addr = 0; 21 - addr;)
        {
            if (currentHour == EEPROM[addr + 1])
            {
                if (currentMinute == EEPROM[addr + 2])
                {
                    // Serial.println("Mon");
                    // Serial.print(EEPROM[addr + 3]);
                    // Serial.print("\t");
                    // Serial.print(EEPROM[addr + 4]);
                    // Serial.print("\t");
                    // Serial.print(EEPROM[addr + 5]);
                    if (EEPROM[addr + 3]) // relay 1
                    {
                        flagEnRelay1 = true;
                        digitalWrite(OUT1, 1);
                        lastDuration1 = t.min;
                        compareDuration1 = EEPROM[addr + 6]; // update duration
                    }
                    if (EEPROM[addr + 4]) // relay 2
                    {
                        flagEnRelay2 = true;
                        digitalWrite(OUT2, 1);
                        lastDuration2 = t.min;
                        compareDuration2 = EEPROM[addr + 6]; // update duration
                    }
                    if (EEPROM[addr + 5]) // relay 3
                    {
                        flagEnRelay3 = true;
                        digitalWrite(OUT3, 1);
                        lastDuration3 = t.min;
                        compareDuration3 = EEPROM[addr + 6]; // update duration
                    }
                    return;
                }
            }
            addr += 7; // Sáng -> Trưa -> Chiều
        }
        break;
    case 3: // Tue
        for (int addr = 21; 42 - addr;)
        {
            if (currentHour == EEPROM[addr + 1])
            {
                if (currentMinute == EEPROM[addr + 2])
                {
                    // Serial.println("Tue");
                    // Serial.print(EEPROM[addr + 3]);
                    // Serial.print("\t");
                    // Serial.print(EEPROM[addr + 4]);
                    // Serial.print("\t");
                    // Serial.print(EEPROM[addr + 5]);
                    if (EEPROM[addr + 3]) // relay 1
                    {
                        flagEnRelay1 = true;
                        digitalWrite(OUT1, 1);
                        lastDuration1 = t.min;
                        compareDuration1 = EEPROM[addr + 6]; // update duration
                    }
                    if (EEPROM[addr + 4]) // relay 2
                    {
                        flagEnRelay2 = true;
                        digitalWrite(OUT2, 1);
                        lastDuration2 = t.min;
                        compareDuration2 = EEPROM[addr + 6]; // update duration
                    }
                    if (EEPROM[addr + 5]) // relay 3
                    {
                        flagEnRelay3 = true;
                        digitalWrite(OUT3, 1);
                        lastDuration3 = t.min;
                        compareDuration3 = EEPROM[addr + 6]; // update duration
                    }
                    return;
                }
            }
            addr += 7;
        }
        break;
    case 4: // Wed
        for (int addr = 42; 63 - addr;)
        {
            if (currentHour == EEPROM[addr + 1])
            {
                if (currentMinute == EEPROM[addr + 2])
                {
                    // Serial.println("Wed");
                    // Serial.print(EEPROM[addr + 3]);
                    // Serial.print("\t");
                    // Serial.print(EEPROM[addr + 4]);
                    // Serial.print("\t");
                    // Serial.print(EEPROM[addr + 5]);
                    if (EEPROM[addr + 3]) // relay 1
                    {
                        flagEnRelay1 = true;
                        digitalWrite(OUT1, 1);
                        lastDuration1 = t.min;
                        compareDuration1 = EEPROM[addr + 6]; // update duration
                    }
                    if (EEPROM[addr + 4]) // relay 2
                    {
                        flagEnRelay2 = true;
                        digitalWrite(OUT2, 1);
                        lastDuration2 = t.min;
                        compareDuration2 = EEPROM[addr + 6]; // update duration
                    }
                    if (EEPROM[addr + 5]) // relay 3
                    {
                        flagEnRelay3 = true;
                        digitalWrite(OUT3, 1);
                        lastDuration3 = t.min;
                        compareDuration3 = EEPROM[addr + 6]; // update duration
                    }
                    return;
                }
            }
            addr += 7;
        }
        break;
    case 5: // Thus
        for (int addr = 63; 84 - addr;)
        {
            if (currentHour == EEPROM[addr + 1])
            {
                if (currentMinute == EEPROM[addr + 2])
                {
                    // Serial.println("Thu");
                    // Serial.print(EEPROM[addr + 3]);
                    // Serial.print("\t");
                    // Serial.print(EEPROM[addr + 4]);
                    // Serial.print("\t");
                    // Serial.print(EEPROM[addr + 5]);
                    if (EEPROM[addr + 3]) // relay 1
                    {
                        flagEnRelay1 = true;
                        digitalWrite(OUT1, 1);
                        lastDuration1 = t.min;
                        compareDuration1 = EEPROM[addr + 6]; // update duration
                    }
                    if (EEPROM[addr + 4]) // relay 2
                    {
                        flagEnRelay2 = true;
                        digitalWrite(OUT2, 1);
                        lastDuration2 = t.min;
                        compareDuration2 = EEPROM[addr + 6]; // update duration
                    }
                    if (EEPROM[addr + 5]) // relay 3
                    {
                        flagEnRelay3 = true;
                        digitalWrite(OUT3, 1);
                        lastDuration3 = t.min;
                        compareDuration3 = EEPROM[addr + 6]; // update duration
                    }
                    return;
                }
            }
            addr += 7;
        }
        break;
    case 6: // Fri
        for (int addr = 84; 105 - addr;)
        {
            if (currentHour == EEPROM[addr + 1])
            {
                if (currentMinute == EEPROM[addr + 2])
                {
                    // Serial.println("Fri");
                    // Serial.print(EEPROM[addr + 3]);
                    // Serial.print("\t");
                    // Serial.print(EEPROM[addr + 4]);
                    // Serial.print("\t");
                    // Serial.print(EEPROM[addr + 5]);
                    if (EEPROM[addr + 3]) // relay 1
                    {
                        flagEnRelay1 = true;
                        digitalWrite(OUT1, 1);
                        lastDuration1 = t.min;
                        compareDuration1 = EEPROM[addr + 6]; // update duration
                    }
                    if (EEPROM[addr + 4]) // relay 2
                    {
                        flagEnRelay2 = true;
                        digitalWrite(OUT2, 1);
                        lastDuration2 = t.min;
                        compareDuration2 = EEPROM[addr + 6]; // update duration
                    }
                    if (EEPROM[addr + 5]) // relay 3
                    {
                        flagEnRelay3 = true;
                        digitalWrite(OUT3, 1);
                        lastDuration3 = t.min;
                        compareDuration3 = EEPROM[addr + 6]; // update duration
                    }
                    return;
                }
            }
            addr += 7;
        }
        break;
    case 7: // Sat
        for (int addr = 105; 126 - addr;)
        {
            if (currentHour == EEPROM[addr + 1])
            {
                if (currentMinute == EEPROM[addr + 2])
                {
                    // Serial.println("Sat");
                    // Serial.print(EEPROM[addr + 3]);
                    // Serial.print("\t");
                    // Serial.print(EEPROM[addr + 4]);
                    // Serial.print("\t");
                    // Serial.print(EEPROM[addr + 5]);
                    if (EEPROM[addr + 3]) // relay 1
                    {
                        flagEnRelay1 = true;
                        digitalWrite(OUT1, 1);
                        lastDuration1 = t.min;
                        compareDuration1 = EEPROM[addr + 6]; // update duration
                    }
                    if (EEPROM[addr + 4]) // relay 2
                    {
                        flagEnRelay2 = true;
                        digitalWrite(OUT2, 1);
                        lastDuration2 = t.min;
                        compareDuration2 = EEPROM[addr + 6]; // update duration
                    }
                    if (EEPROM[addr + 5]) // relay 3
                    {
                        flagEnRelay3 = true;
                        digitalWrite(OUT3, 1);
                        lastDuration3 = t.min;
                        compareDuration3 = EEPROM[addr + 6]; // update duration
                    }
                    return;
                }
            }
            addr += 7;
        }
        break;
    case 1: // Sun
        for (int addr = 126; 147 - addr;)
        {
            if (currentHour == EEPROM[addr + 1])
            {
                if (currentMinute == EEPROM[addr + 2])
                {
                    // Serial.println("Sun");
                    // Serial.print(EEPROM[addr + 3]);
                    // Serial.print("\t");
                    // Serial.print(EEPROM[addr + 4]);
                    // Serial.print("\t");
                    // Serial.print(EEPROM[addr + 5]);
                    if (EEPROM[addr + 3]) // relay 1
                    {
                        flagEnRelay1 = true;
                        digitalWrite(OUT1, 1);
                        lastDuration1 = t.min;
                        compareDuration1 = EEPROM[addr + 6]; // update duration
                    }
                    if (EEPROM[addr + 4]) // relay 2
                    {
                        flagEnRelay2 = true;
                        digitalWrite(OUT2, 1);
                        lastDuration2 = t.min;
                        compareDuration2 = EEPROM[addr + 6]; // update duration
                    }
                    if (EEPROM[addr + 5]) // relay 3
                    {
                        flagEnRelay3 = true;
                        digitalWrite(OUT3, 1);
                        lastDuration3 = t.min;
                        compareDuration3 = EEPROM[addr + 6]; // update duration
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
    int iCusAddr = 138;
    for (int i = 0; EEPROM[147] - i;)
    { // loop num of length times
        do
        { // pass null event to generate iCusAddr
            iCusAddr += 10;
        } while ((EEPROM[iCusAddr] == 0) && (iCusAddr < 1024));

        if (currentMinute == EEPROM[iCusAddr + 5])
            if (currentHour == EEPROM[iCusAddr + 4])
                if (currentMonthDay == EEPROM[iCusAddr])
                    if (currentMonth == EEPROM[iCusAddr + 1])
                        if ((currentYear / 100) == EEPROM[iCusAddr + 2])
                            if ((currentYear % 100) == EEPROM[iCusAddr + 3]) // an event is match
                            {
                                // debug
                                // Serial.println();
                                // Serial.print("Alarm: ");
                                // Serial.print(EEPROM[iCusAddr + 6]);
                                // Serial.print("\t");
                                // Serial.print(EEPROM[iCusAddr + 7]);
                                // Serial.print("\t");
                                // Serial.print(EEPROM[iCusAddr + 8]);

                                // check bit if enable relay or not
                                if (EEPROM[iCusAddr + 6])
                                {
                                    flagEnRelay1 = true;
                                    digitalWrite(OUT1, 1);
                                    lastDuration1 = t.min;
                                    compareDuration1 = EEPROM[iCusAddr + 9]; // update duration
                                }
                                if (EEPROM[iCusAddr + 7])
                                {
                                    flagEnRelay2 = true;
                                    digitalWrite(OUT2, 1);
                                    lastDuration2 = t.min;
                                    compareDuration2 = EEPROM[iCusAddr + 9]; // update duration
                                }
                                if (EEPROM[iCusAddr + 8])
                                {
                                    flagEnRelay3 = true;
                                    digitalWrite(OUT3, 1);
                                    lastDuration3 = t.min;
                                    compareDuration3 = EEPROM[iCusAddr + 9]; // update duration
                                }

                                // delete this match events
                                EEPROM.write(iCusAddr, 0);
                                EEPROM[147] -= 1;
                                iCusEvents = EEPROM[147];
                                iCusAddressEEProm = 138;

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
