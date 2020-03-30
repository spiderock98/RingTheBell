#include "misc.h"
#include "repeat.h"
#include "custom.h"

struct ts t;

volatile int16_t chosenDayOfWeek = -1, iAddressEEProm = -7, iCusAddressEEProm = 138, numOfEvents = 0;
byte arrTick[256], iCusEvents = EEPROM[147];
bool flagRepeatSetting = false, flagCusSetting = false;

//TODO: ghi arr vào eeprom confirm

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
    if ((key == 'A') && !flagRepeatSetting)
    {
      ++chosenDayOfWeek;
      iAddressEEProm += 7;
      if (chosenDayOfWeek == 21)
      {
        //TODO: return home screen
        lcd.clear();
        chosenDayOfWeek = -1;
        iAddressEEProm = -7;
      }
      repeaterInterface();
    }
    else if (key == 'B')
    {
      if (!flagRepeatSetting)
        repeaterSetValue();
    }
    // duyệt custom events
    else if ((key == 'D') && !flagCusSetting)
    {
      Serial.print("EEPROM[147]: ");
      Serial.println(EEPROM[147]);
      if (EEPROM[147] == 0)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Khong co su kien");
        lcd.setCursor(1, 1);
        lcd.print("GIU D de them");
        delay(2000);
        //TODO: return home screen
        lcd.clear();
        return;
      }
      do
      { // pass null event to generate iCusAddressEEProm
        iCusAddressEEProm += 10;
        Serial.print("iCusAddressEEProm: ");
        Serial.println(iCusAddressEEProm);
      } while ((EEPROM[iCusAddressEEProm] == 0) && (iCusAddressEEProm < 1024) && iCusEvents);

      if (!iCusEvents)
      {
        iCusEvents = EEPROM[147];
        iCusAddressEEProm = 138;
        Serial.print("iCusAddressEEProm: ");
        Serial.println(iCusAddressEEProm);
        lcd.clear();
        return;
      }

      --iCusEvents;

      customInterface();
    }
    break;

    //    case RELEASED:
    //      if (key == '*') {
    //        digitalWrite(ledPin, ledPin_state);   // Restore LED state from before it started blinking.
    //        blink = false;
    //      }
    //      break;

    // thêm event mới
  case HOLD:
    if ((key == '#'))
    {
      if (!flagCusSetting)
      {
        iCusAddressEEProm = 138;
        do
        { // find null event >> avoid EEProm diskfrag
          iCusAddressEEProm += 10;
        } while ((EEPROM[iCusAddressEEProm] != 0) && (iCusAddressEEProm < 1024));

        customSetValue();
      }
      else if (flagCusSetting)
      {
        flagCusSetting = false;
        lcd.clear();
      }
    }
    break;
  }
}
