#include "repeat.h"

extern byte arrTick[256];
extern volatile int16_t chosenDayOfWeek, iAddressEEProm;
extern bool flagRepeatSetting, flagRepeatView, flagHomeView;

// UI when press 'A'
void repeaterInterface()
{
    flagRepeatView = true;
    flagHomeView = false;

    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Th");
    lcd.printByte(2);
    lcd.setCursor(8, 0);
    lcd.print("A");
    lcd.setCursor(11, 0);
    lcd.print("B");
    lcd.setCursor(14, 0);
    lcd.print("C");
    lcd.setCursor(2, 1);
    lcd.print(":");
    lcd.setCursor(6, 1);
    lcd.print("K");
    lcd.printByte(3);
    lcd.print("o");
    lcd.setCursor(13, 1);
    lcd.print("ph");

    // 0-1-2 <<>> sángt2-trưat2-chiềut2 etc.
    switch (chosenDayOfWeek)
    {
    case 0:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Sa");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("2");
        EEPr2DailyUI();
        break;
    case 1:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Tr");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("2");
        EEPr2DailyUI();
        break;
    case 2:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Ch");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("2");
        EEPr2DailyUI();
        break;
    case 3:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Sa");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("3");
        EEPr2DailyUI();
        break;
    case 4:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Tr");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("3");
        EEPr2DailyUI();
        break;
    case 5:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Ch");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("3");
        EEPr2DailyUI();
        break;
    case 6:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Sa");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("4");
        EEPr2DailyUI();
        break;
    case 7:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Tr");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("4");
        EEPr2DailyUI();
        break;
    case 8:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Ch");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("4");
        EEPr2DailyUI();
        break;
    case 9:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Sa");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("5");
        EEPr2DailyUI();
        break;
    case 10:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Tr");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("5");
        EEPr2DailyUI();
        break;
    case 11:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Ch");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("5");
        EEPr2DailyUI();
        break;
    case 12:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Sa");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("6");
        EEPr2DailyUI();
        break;
    case 13:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Tr");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("6");
        EEPr2DailyUI();
        break;
    case 14:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Ch");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("6");
        EEPr2DailyUI();
        break;
    case 15:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Sa");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("7");
        EEPr2DailyUI();
        break;
    case 16:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Tr");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("7");
        EEPr2DailyUI();
        break;
    case 17:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Ch");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("7");
        EEPr2DailyUI();
        break;
    case 18:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Sa");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("Cn");
        EEPr2DailyUI();
        break;
    case 19:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Tr");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("Cn");
        EEPr2DailyUI();
        break;
    case 20:
        lcd.setCursor(addressCol::TimeInDay, 0);
        lcd.print("Ch");
        lcd.setCursor(addressCol::DayOfWeek, 0);
        lcd.print("Cn");
        EEPr2DailyUI();
        break;
    }
}

// init arrTick and load eepr data to corres ui times in day arrTick
void EEPr2DailyUI()
{
    for (int i = iAddressEEProm; iAddressEEProm + 7 - i;)
    {
        arrTick[i] = EEPROM[i]; // begin declare arrTick from EEPROM
        switch (i - iAddressEEProm)
        {
        case 1:
            lcd.setCursor(addressCol::HOUR, 1);
            lcd.print(arrTick[i]);
            break;
        case 2:
            lcd.setCursor(addressCol::MIN, 1);
            lcd.print(arrTick[i]);
            break;
        case 3:
            lcd.setCursor(addressCol::RELAY1, 0);
            lcd.print(arrTick[i]);
            break;
        case 4:
            lcd.setCursor(addressCol::RELAY2, 0);
            lcd.print(arrTick[i]);
            break;
        case 5:
            lcd.setCursor(addressCol::RELAY3, 0);
            lcd.print(arrTick[i]);
            break;
        case 6:
            lcd.setCursor(addressCol::DURATION, 1);
            lcd.print(arrTick[i]);
            break;
        default:
            break;
        }
        ++i;
    }
    lcd.setCursor(addressCol::DayOfWeek, 0);
    lcd.cursor();
    lcd.blink();
}

// setter
void repeaterSetValue()
{
    flagRepeatSetting = true;
    flagHomeView = false;
    byte decVal;
    char charVal, key;

lbRelay1:
    lcd.setCursor(addressCol::RELAY1, 0);
    charVal = keypad.waitForKey(); // blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if (charVal != '0')
        {
            lcd.print(1);
            arrTick[iAddressEEProm + 3] = 1; // just store 0 or 1
        }
        else
        {
            lcd.print(0);
            arrTick[iAddressEEProm + 3] = 0; // just store 0 or 1
        }
    }
    else if (charVal == 'B')
        goto lbRelay2;
    else
        goto lbRelay1;

lbRelay2:
    lcd.setCursor(addressCol::RELAY2, 0);
    charVal = keypad.waitForKey(); //blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if (charVal != '0')
        {
            lcd.print(1);
            arrTick[iAddressEEProm + 4] = 1; // just store 0 or 1
        }
        else
        {
            lcd.print(0);
            arrTick[iAddressEEProm + 4] = 0; // just store 0 or 1
        }
    }
    else if (charVal == 'B')
        goto lbRelay3;
    else if (charVal == 'C')
        goto lbRelay1;
    else
        goto lbRelay2;

lbRelay3:
    lcd.setCursor(addressCol::RELAY3, 0);
    charVal = keypad.waitForKey(); //blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if (charVal != '0')
        {
            lcd.print(1);
            arrTick[iAddressEEProm + 5] = 1; // just store 0 or 1
        }
        else
        {
            lcd.print(0);
            arrTick[iAddressEEProm + 5] = 0; // just store 0 or 1
        }
    }
    else if (charVal == 'B')
        goto lbHour;
    else if (charVal == 'C')
        goto lbRelay2;
    else
        goto lbRelay3;

lbHour:
    // hang chuc
    lcd.setCursor(addressCol::HOUR, 1);
    charVal = keypad.waitForKey(); //blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        lcd.print(charVal);
    }
    else if (charVal == 'B')
        goto lbMinute;
    else if (charVal == 'C')
        goto lbRelay3;
    else
        goto lbHour;

    // hang don vi
    charVal = keypad.waitForKey(); //blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if (decVal >= 24)
            goto lbHour;
        lcd.print(charVal);
        arrTick[iAddressEEProm + 1] = decVal; // store here not store in hang don vi
    }
    else if (charVal == 'B')
        goto lbMinute;
    else if (charVal == 'C')
        goto lbRelay3;
    else
        goto lbHour;

lbMinute:
    // hang chuc
    lcd.setCursor(addressCol::MIN, 1);
    charVal = keypad.waitForKey(); //blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        lcd.print(charVal);
    }
    else if (charVal == 'B')
        goto lbDuration;
    else if (charVal == 'C')
        goto lbHour;
    else
        goto lbMinute;
    // hang don vi
    charVal = keypad.waitForKey(); //blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if (decVal >= 60)
            goto lbMinute;
        lcd.print(charVal);
        arrTick[iAddressEEProm + 2] = decVal;
    }
    else if (charVal == 'B')
        goto lbDuration;
    else if (charVal == 'C')
        goto lbHour;
    else
        goto lbMinute;

lbDuration:
    // hang chuc
    lcd.setCursor(addressCol::DURATION, 1);
    charVal = keypad.waitForKey(); //blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        lcd.print(charVal);
    }
    else if (charVal == 'C')
        goto lbMinute;
    else
        goto lbDuration;

    // hang don vi
    charVal = keypad.waitForKey(); //blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if ((decVal > 60) || (decVal == 0))
            goto lbDuration;
        lcd.print(charVal);
        arrTick[iAddressEEProm + 6] = decVal;
    }
    else if (charVal == 'C')
        goto lbMinute;
    else
        goto lbDuration;

    // set arrTick to EEPr
    for (int i = iAddressEEProm; iAddressEEProm + 7 - i;)
    {
        EEPROM.update(i, arrTick[i]);
        delay(5);
        ++i;
    }

    // ready for next UI daily
    delay(1000);
    ++chosenDayOfWeek;
    iAddressEEProm += 7;

    // in case we set value in chiều CN
    if (chosenDayOfWeek == 21)
    {
        //TODO: return home screen
        lcd.clear();
        chosenDayOfWeek = -1;
        iAddressEEProm = -7;
        return;
    }
    flagRepeatSetting = false;
    repeaterInterface();
}