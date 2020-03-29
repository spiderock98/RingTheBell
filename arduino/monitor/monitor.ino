#include <Keypad.h>
#include <Wire.h>
#include <ds3231.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args) write(args);
#else
#define printByte(args) print(args, BYTE);
#endif

struct ts t;

LiquidCrystal_I2C lcd(0x3F, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

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

void lcdDefaultInterface()
{
  uint8_t bell[8] = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
  uint8_t clock[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
  uint8_t u71[8] = {0x04, 0x09, 0x03, 0x12, 0x12, 0x12, 0x0C, 0x00};
  uint8_t e1[8] = {0x08, 0x10, 0x0E, 0x11, 0x1F, 0x10, 0x0E, 0x00};

  lcd.init();
  lcd.backlight();

  lcd.createChar(0, bell);
  lcd.createChar(1, clock);
  lcd.createChar(2, u71);
  lcd.createChar(3, e1);

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

enum addressCol
{
  DayOfWeek = 5,
  HOUR = 0,
  MIN = 3,
  RELAY1 = 9,
  RELAY2 = 12,
  RELAY3 = 15,
  DURATION = 10
};
// addressCol addressColum;
// enum menuDayofWeek
// {
//   Mon,
//   Tue,
//   Wed,
//   Thu,
//   Fri,
//   Sat,
//   Sun
// };

// struct datee
// {

// }

volatile int8_t chosenDayOfWeek = 1, iAddressEEProm = -7;
byte arrTick[256];
bool flagSetting = false;

void initRepeaterInterface()
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

//TODO: ghi arr vào eeprom confirm

// void repeaterSetter()
// {
// }
void repeaterSetValue()
{
  // int index = -1;
  flagSetting = true;
  byte decVal;
  char charVal, key;

lbRelay1:
  // ++index;
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
  // ++index;
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
  // ++index;
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
  // ++index;
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
  // ++index;
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
  // ++index;
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
    if (decVal > 60)
      goto lbDuration;
    lcd.print(charVal);
    arrTick[iAddressEEProm + 6] = decVal;
  }
  else if (charVal == 'C')
    goto lbMinute;
  else
    goto lbDuration;

  //TODO: Confirmed setting
  flagSetting = false;
  delay(1000);
  ++chosenDayOfWeek;
  iAddressEEProm += 7;
  if (chosenDayOfWeek == 9)
  {
    //TODO: return home screen
    lcd.clear();
    chosenDayOfWeek = 1;
    iAddressEEProm = 0;
    return;
  }

  repeaterInterface();
}

void repeaterInterface()
{
  lcd.setCursor(1, 0);
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

  switch (chosenDayOfWeek)
  {
  case 2:
    lcd.setCursor(addressCol::DayOfWeek, 0);
    lcd.print("2");
    initRepeaterInterface();
    break;
  case 3:
    lcd.setCursor(addressCol::DayOfWeek, 0);
    lcd.print("3");
    initRepeaterInterface();
    break;
  case 4:
    lcd.setCursor(addressCol::DayOfWeek, 0);
    lcd.print("4");
    initRepeaterInterface();
    break;
  case 5:
    lcd.setCursor(addressCol::DayOfWeek, 0);
    lcd.print("5");
    initRepeaterInterface();
    break;
  case 6:
    lcd.setCursor(addressCol::DayOfWeek, 0);
    lcd.print("6");
    initRepeaterInterface();
    break;
  case 7:
    lcd.setCursor(addressCol::DayOfWeek, 0);
    lcd.print("7");
    initRepeaterInterface();
    break;
  case 8:
    lcd.setCursor(addressCol::DayOfWeek, 0);
    lcd.print("CN");
    initRepeaterInterface();
    break;
  }
}

// void loadEEPROM(addressCol timeStick)
// {
//   switch (timeStick)
//   {
//   case DayOfWeek:
//     /* code */
//     break;

//   default:
//     break;
//   }
// }

byte char2byte(char key)
{
  return (byte)key - 48;
}

void setup()
{
  Serial.begin(9600);

  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  keypad.addEventListener(keypadEvent);
  lcdDefaultInterface();
}

void loop()
{
  // DS3231_get(&t);

  char key = keypad.getKey();
  // if (key)
  // {
  //   lcd.backlight();
  //   lcd.setCursor(0, 0);
  //   lcd.print(char2byte(key));
  // }

  // lcd.setCursor(0, 1);
  // lcd.print(t.hour);
  // lcd.print(":");
  // lcd.print(t.min);
  // lcd.print(":");
  // lcd.print(t.sec);
}
void keypadEvent(KeypadEvent key)
{
  switch (keypad.getState())
  {
  case PRESSED:
    if ((key == 'A') && !flagSetting)
    {
      ++chosenDayOfWeek;
      iAddressEEProm += 7;
      if (chosenDayOfWeek == 9)
      {
        //TODO: return home screen
        lcd.clear();
        chosenDayOfWeek = 1;
        iAddressEEProm = 0;
      }

      repeaterInterface();
    }
    else if (key == 'B')
    {
      if (!flagSetting)
        repeaterSetValue();
    }
    break;

    //    case RELEASED:
    //      if (key == '*') {
    //        digitalWrite(ledPin, ledPin_state);   // Restore LED state from before it started blinking.
    //        blink = false;
    //      }
    //      break;

    // case HOLD:
    //   if (key == '*')
    //   {
    //     Serial.println("Cleanr");
    //     lcd.clear();
    //   }

    //   break;
  }
}
