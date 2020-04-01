#include "misc.h"
#include "repeat.h"
#include "custom.h"

struct ts t;
volatile int16_t chosenDayOfWeek = -1, iAddressEEProm = -7, iCusAddressEEProm = 138, numOfEvents = 0;
byte arrTick[256], iCusEvents = EEPROM[147];
bool flagHomeView = true, flagRepeatSetting = false, flagCusSetting = false, flagCusView = false, flagRepeatView = false;
uint8_t lastMin = 0;

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
  DS3231_get(&t);

  char key = keypad.getKey();

  if (!flagCusSetting && !flagRepeatSetting && !flagCusView && !flagRepeatView)
    lcdHomeScreen();

  if (t.min - lastMin)
  {
    alarm();
    lastMin = t.min;
  }
}

void keypadEvent(KeypadEvent key)
{
  switch (keypad.getState())
  {
  case PRESSED:
    if ((key == 'A') && !flagRepeatSetting && !flagCusSetting)
    {
      ++chosenDayOfWeek;
      iAddressEEProm += 7;
      if (chosenDayOfWeek == 21)
      {
        //TODO: return home screen
        lcd.clear();
        flagRepeatView = false;
        chosenDayOfWeek = -1;
        iAddressEEProm = -7;
        return;
      }
      repeaterInterface();
    }
    else if (key == 'B')
    {
      if (!flagRepeatSetting && flagRepeatView)
        repeaterSetValue();
    }

    // duyệt custom events
    else if ((key == 'D') && !flagRepeatView && !flagCusSetting)
    {
      if (EEPROM[147] == 0)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Khong co su kien");
        lcd.setCursor(1, 1);
        lcd.print("GIU D de them");
        delay(2000); // chờ nhấn giữ D
        //TODO: return home screen
        lcd.clear();
        return;
      }
      do
      { // pass null event to generate iCusAddressEEProm
        iCusAddressEEProm += 10;
        Serial.println();
        Serial.print("iCusAddressEEProm: ");
        Serial.println(iCusAddressEEProm);
        Serial.println(iCusEvents);
      } while ((EEPROM[iCusAddressEEProm] == 0) && (iCusAddressEEProm < 1024) && iCusEvents);

      if (!iCusEvents)
      {
        iCusEvents = EEPROM[147];
        iCusAddressEEProm = 138;
        flagCusView = false;
        lcd.clear();
        return;
      }

      --iCusEvents;

      customInterface();
    }
    break;

    // thêm event mới
  case HOLD:
    if ((key == 'D'))
    {
      if (!flagCusSetting && !flagRepeatSetting && !flagRepeatView) // just in view can add event
      {
        // fix when hold d >> you must pass PRESSED D and get these stuff wrong
        iCusAddressEEProm = 138;
        ++iCusEvents;
        flagCusView = false;

        do
        { // find null event >> avoid EEProm diskfrag
          iCusAddressEEProm += 10;
        } while ((EEPROM[iCusAddressEEProm] != 0) && (iCusAddressEEProm < 1024));

        customSetValue();

        // Serial.println(flagCusView);
        // Serial.println(flagCusSetting);
        // Serial.println(flagRepeatSetting);
        // Serial.println(flagRepeatView);
      }
      // else if (flagCusSetting)
      // {
      //   --iCusEvents;
      //   flagCusSetting = false;
      //   iCusAddressEEProm = 138;
      //   //TODO: return home
      //   lcd.clear();
      //   // Serial.println(flagCusView);
      //   // Serial.println(flagCusSetting);
      //   // Serial.println(flagRepeatSetting);
      //   // Serial.println(flagRepeatView);
      // }
    }
    // hold #
    else if (key == '#')
    {
      if (flagCusView)
      {
        customDeleteValue();
        flagCusView = false;
      }
      break;
    }
  }
}