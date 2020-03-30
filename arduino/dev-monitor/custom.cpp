#include "custom.h"

extern byte arrTick[256], iCusEvents;
extern volatile int16_t iCusAddressEEProm, numOfEvents;
extern bool flagCusSetting;

void customSetValue()
{
    blankCusInterface();
    lcd.cursor();
    lcd.blink();

    flagCusSetting = true;
    byte decVal;
    char charVal, key;

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

    //TODO: Confirmed setting
    delay(1000);

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Them Su Kien ?");

    lcd.setCursor(0, 1);
    lcd.printByte(4); //checked symbol
    lcd.print(" A:OK");

    lcd.setCursor(9, 1);
    lcd.printByte(5); // cancle symbol
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

            delay(5);
            // Serial.print("Total: ");
            // Serial.println(EEPROM[147]);
            EEPROM.update(i, arrTick[i]);
            delay(5);
            ++i;
        }
        EEPROM[147] += 1;
        ++iCusEvents;
        iCusAddressEEProm = 138;
    }
    else
        goto lbConfirm;

    flagCusSetting = false;
    lcd.clear();
}

void blankCusInterface()
{
    lcd.clear();
    lcd.home();
    lcd.print("DD-MM-YYYY HH:MM");
    // lcd.setCursor(2, 0);
    // lcd.print('-');
    // lcd.setCursor(5, 0);
    // lcd.print('-');
    // lcd.setCursor(13, 0);
    // lcd.print(':');
    lcd.setCursor(0, 1);
    lcd.print("A  B  C  K");
    // lcd.print("A");
    // lcd.setCursor(3, 1);
    // lcd.print("B");
    // lcd.setCursor(6, 1);
    // lcd.print("C");
    // lcd.setCursor(9, 1);
    // lcd.print("K");
    lcd.printByte(3);
    lcd.print("o MMp");
    // lcd.setCursor(15, 1);
    // lcd.print("p");
}
void _blankCusInterface()
{
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
    _blankCusInterface();
    initCustomInterface();

    // switch (chosenDayOfWeek)
    // {
    // case 0:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Sa");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("2");
    //     initRepeaterInterface();
    //     break;
    // case 1:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Tr");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("2");
    //     initRepeaterInterface();
    //     break;
    // case 2:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Ch");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("2");
    //     initRepeaterInterface();
    //     break;
    // case 3:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Sa");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("3");
    //     initRepeaterInterface();
    //     break;
    // case 4:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Tr");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("3");
    //     initRepeaterInterface();
    //     break;
    // case 5:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Ch");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("3");
    //     initRepeaterInterface();
    //     break;
    // case 6:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Sa");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("4");
    //     initRepeaterInterface();
    //     break;
    // case 7:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Tr");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("4");
    //     initRepeaterInterface();
    //     break;
    // case 8:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Ch");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("4");
    //     initRepeaterInterface();
    //     break;
    // case 9:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Sa");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("5");
    //     initRepeaterInterface();
    //     break;
    // case 10:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Tr");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("5");
    //     initRepeaterInterface();
    //     break;
    // case 11:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Ch");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("5");
    //     initRepeaterInterface();
    //     break;
    // case 12:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Sa");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("6");
    //     initRepeaterInterface();
    //     break;
    // case 13:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Tr");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("6");
    //     initRepeaterInterface();
    //     break;
    // case 14:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Ch");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("6");
    //     initRepeaterInterface();
    //     break;
    // case 15:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Sa");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("7");
    //     initRepeaterInterface();
    //     break;
    // case 16:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Tr");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("7");
    //     initRepeaterInterface();
    //     break;
    // case 17:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Ch");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("7");
    //     initRepeaterInterface();
    //     break;
    // case 18:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Sa");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("Cn");
    //     initRepeaterInterface();
    //     break;
    // case 19:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Tr");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("Cn");
    //     initRepeaterInterface();
    //     break;
    // case 20:
    //     lcd.setCursor(addressCol::TimeInDay, 0);
    //     lcd.print("Ch");
    //     lcd.setCursor(addressCol::DayOfWeek, 0);
    //     lcd.print("Cn");
    //     initRepeaterInterface();
    //     break;
    // }
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