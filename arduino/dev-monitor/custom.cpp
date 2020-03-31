#include "custom.h"

extern byte arrTick[256], iCusEvents;
extern volatile int16_t iCusAddressEEProm, numOfEvents;
extern bool flagCusSetting, flagCusView, flagHomeView;

void customDeleteValue()
{
    flagCusSetting = true;
    flagHomeView = false;

    char charVal;

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("XOA su kien ?");
    lcd.setCursor(0, 1);
    lcd.printByte(4); //checked symbol
    lcd.print(" A:OK");
    lcd.setCursor(9, 1);
    lcd.printByte(5); // cancle symbol
    lcd.setCursor(10, 1);
    lcd.print(" D:Huy");

lbReConfirm:
    charVal = keypad.waitForKey(); //blocking
    if (charVal == 'D')
        ;
    else if (charVal == 'A')
    {
        EEPROM.write(iCusAddressEEProm, 0);
        EEPROM[147] -= 1;
        --iCusEvents;
    }
    else
        goto lbReConfirm;

    flagCusSetting = false;
    flagCusView = false;
    //TODO: return home
    lcd.clear();
    iCusAddressEEProm = 138;
}
void customSetValue()
{
    blankCusInterface();

    flagCusSetting = true;
    flagHomeView = false;

    byte decVal;
    char charVal;

lbDayOfMonth:
    // hang chuc
    lcd.setCursor(cusAddressCol::cusDayOfMonth, 0);
    charVal = keypad.waitForKey(); //blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        lcd.print(charVal);
    }
    else if (charVal == 'B')
        goto lbMonth;
    else
        goto lbDayOfMonth;

    // hang don vi
    charVal = keypad.waitForKey(); //blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if ((decVal >= 32) || (decVal == 0))
            goto lbDayOfMonth;
        lcd.print(charVal);
        arrTick[iCusAddressEEProm] = decVal; // store here not store in hang don vi
    }
    else if (charVal == 'B')
        goto lbMonth;
    else
        goto lbDayOfMonth;

lbMonth:
    // hang chuc
    lcd.setCursor(cusAddressCol::cusMONTH, 0);
    charVal = keypad.waitForKey(); //blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        lcd.print(charVal);
    }
    else if (charVal == 'A')
        goto lbYear;
    else if (charVal == 'B')
        goto lbDayOfMonth;
    else
        goto lbMonth;

    // hang chuc
    charVal = keypad.waitForKey(); //blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if ((decVal >= 13) || (decVal == 0))
            goto lbMonth;
        lcd.print(charVal);
        arrTick[iCusAddressEEProm + 1] = decVal; // store here not store in hang chuc
    }
    else if (charVal == 'A')
        goto lbYear;
    else if (charVal == 'B')
        goto lbDayOfMonth;
    else
        goto lbMonth;

lbYear:
    // hang nghin
    lcd.setCursor(cusAddressCol::cusYEAR1, 0);
    charVal = keypad.waitForKey(); //blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        lcd.print(charVal);
    }
    else if (charVal == 'A')
        goto lbHour;
    else if (charVal == 'B')
        goto lbMonth;
    else
        goto lbYear;

    // hang tram
    charVal = keypad.waitForKey(); //blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if (decVal <= 19)
            goto lbYear;
        lcd.print(charVal);
        arrTick[iCusAddressEEProm + 2] = decVal;
    }
    else if (charVal == 'A')
        goto lbHour;
    else if (charVal == 'B')
        goto lbMonth;
    else
        goto lbYear;

    // hang chuc
    charVal = keypad.waitForKey(); //blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        lcd.print(charVal);
    }
    else if (charVal == 'A')
        goto lbHour;
    else if (charVal == 'B')
        goto lbMonth;
    else
        goto lbYear;

    // hang don vi
    charVal = keypad.waitForKey(); //blocking
    decVal = decVal * 10 + char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        lcd.print(charVal);
        arrTick[iCusAddressEEProm + 3] = decVal;
    }
    else if (charVal == 'A')
        goto lbHour;
    else if (charVal == 'B')
        goto lbMonth;
    else
        goto lbYear;
lbHour:
    // hang chuc
    lcd.setCursor(cusAddressCol::cusHOUR, 0);
    charVal = keypad.waitForKey(); //blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        lcd.print(charVal);
    }
    else if (charVal == 'B')
        goto lbMinute;
    else if (charVal == 'C')
        goto lbYear;
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
        arrTick[iCusAddressEEProm + 4] = decVal;
    }
    else if (charVal == 'B')
        goto lbMinute;
    else if (charVal == 'C')
        goto lbYear;
    else
        goto lbHour;

lbMinute:
    // hang chuc
    lcd.setCursor(cusAddressCol::cusMIN, 0);
    charVal = keypad.waitForKey(); //blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        lcd.print(charVal);
    }
    else if (charVal == 'B')
        goto lbRelay1;
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
        arrTick[iCusAddressEEProm + 5] = decVal;
    }
    else if (charVal == 'B')
        goto lbRelay1;
    else if (charVal == 'C')
        goto lbHour;
    else
        goto lbMinute;

lbRelay1:
    lcd.setCursor(cusAddressCol::cusRELAY1, 1);
    charVal = keypad.waitForKey(); // blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if (charVal != '0')
        {
            lcd.print(1);
            arrTick[iCusAddressEEProm + 6] = 1; // just store 0 or 1
        }
        else
        {
            lcd.print(0);
            arrTick[iCusAddressEEProm + 6] = 0; // just store 0 or 1
        }
    }
    else if (charVal == 'B')
        goto lbRelay2;
    else if (charVal == 'C')
        goto lbMinute;
    else
        goto lbRelay1;

lbRelay2:
    lcd.setCursor(cusAddressCol::cusRELAY2, 1);
    charVal = keypad.waitForKey(); //blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if (charVal != '0')
        {
            lcd.print(1);
            arrTick[iCusAddressEEProm + 7] = 1; // just store 0 or 1
        }
        else
        {
            lcd.print(0);
            arrTick[iCusAddressEEProm + 7] = 0; // just store 0 or 1
        }
    }
    else if (charVal == 'B')
        goto lbRelay3;
    else if (charVal == 'C')
        goto lbRelay1;
    else
        goto lbRelay2;

lbRelay3:
    lcd.setCursor(cusAddressCol::cusRELAY3, 1);
    charVal = keypad.waitForKey(); //blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        if (charVal != '0')
        {
            lcd.print(1);
            arrTick[iCusAddressEEProm + 8] = 1; // just store 0 or 1
        }
        else
        {
            lcd.print(0);
            arrTick[iCusAddressEEProm + 8] = 0; // just store 0 or 1
        }
    }
    else if (charVal == 'B')
        goto lbDuration;
    else if (charVal == 'C')
        goto lbRelay2;
    else
        goto lbRelay3;

lbDuration:
    // hang chuc
    lcd.setCursor(cusAddressCol::cusDURATION, 1);
    charVal = keypad.waitForKey(); //blocking
    decVal = char2byte(charVal);
    if (!isSpecialChar(charVal))
    {
        lcd.print(charVal);
    }
    else if (charVal == 'C')
        goto lbRelay3;
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
        arrTick[iCusAddressEEProm + 9] = decVal;
    }
    else if (charVal == 'C')
        goto lbMinute;
    else
        goto lbDuration;

    delay(1000);

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Them Su Kien ?");

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
        for (int i = iCusAddressEEProm; iCusAddressEEProm + 10 - i;)
        {
            EEPROM.update(i, arrTick[i]);
            delay(5);
            ++i;
        }
        EEPROM[147] += 1;
        ++iCusEvents;
    }
    else
        goto lbConfirm;

    iCusAddressEEProm = 138;
    flagCusSetting = false;
    //TODO: return home
    lcd.clear();

    // lcdHomeScreen();
}

void blankCusInterface()
{
    lcd.cursor();
    lcd.blink();
    lcd.clear();
    lcd.home();
    lcd.print("DD-MM-YYYY HH:MM");
    lcd.setCursor(0, 1);
    lcd.print("A  B  C  K");
    lcd.printByte(3);
    lcd.print("o MMp");
}

void _blankCusInterface()
{
    lcd.noCursor();
    lcd.noBlink();
    lcd.clear();
    lcd.home();
    lcd.print("  -  -       :  ");
    lcd.setCursor(0, 1);
    lcd.print("A  B  C  K");
    lcd.printByte(3);
    lcd.print("o   p");
}

void customInterface()
{
    flagCusView = true;
    flagHomeView = false;
    _blankCusInterface();
    initCustomInterface();
}

void initCustomInterface()
{
    for (int i = iCusAddressEEProm; iCusAddressEEProm + 10 - i;)
    {
        arrTick[i] = EEPROM[i]; // begin declare arrTick from EEPROM
        switch (i - iCusAddressEEProm)
        {
        case 0:
            lcd.setCursor(cusAddressCol::cusDayOfMonth, 0);
            lcd.print(arrTick[i]);
            break;
        case 1:
            lcd.setCursor(cusAddressCol::cusMONTH, 0);
            lcd.print(arrTick[i]);
            break;
        case 2:
            lcd.setCursor(cusAddressCol::cusYEAR1, 0);
            lcd.print(arrTick[i]);
            break;
        case 3:
            lcd.setCursor(cusAddressCol::cusYEAR2, 0);
            lcd.print(arrTick[i]);
            break;
        case 4:
            lcd.setCursor(cusAddressCol::cusHOUR, 0);
            lcd.print(arrTick[i]);
            break;
        case 5:
            lcd.setCursor(cusAddressCol::cusMIN, 0);
            lcd.print(arrTick[i]);
            break;
        case 6:
            lcd.setCursor(cusAddressCol::cusRELAY1, 1);
            lcd.print(arrTick[i]);
            break;
        case 7:
            lcd.setCursor(cusAddressCol::cusRELAY2, 1);
            lcd.print(arrTick[i]);
            break;
        case 8:
            lcd.setCursor(cusAddressCol::cusRELAY3, 1);
            lcd.print(arrTick[i]);
            break;
        case 9:
            lcd.setCursor(cusAddressCol::cusDURATION, 1);
            lcd.print(arrTick[i]);
            break;

        default:
            break;
        }
        ++i;
    }
}