#include "misc.h"
#include "repeat.h"
#include "custom.h"

struct ts t;
volatile int16_t chosenDayOfWeek = -1, iAddressEEProm = -7, iCusAddressEEProm = 138, numOfEvents = 0;
byte arrTick[256], iCusEvents = EEPROM[147];

bool flagHomeView = true, flagRepeatSetting = false, flagCusSetting = false, flagCusView = false, flagRepeatView = false, flagEnRelay1 = false, flagEnRelay2 = false, flagEnRelay3 = false;

uint8_t lastMin = 0;
uint8_t lastDuration1 = 0, lastDuration2 = 0, lastDuration3 = 0;
uint8_t compareDuration1 = 5, compareDuration2 = 5, compareDuration3 = 5; //minutes

void setup()
{
  Serial.begin(9600);

  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(RF1, INPUT);
  pinMode(RF2, INPUT);
  pinMode(RF3, INPUT);

  digitalWrite(OUT1, 0);
  digitalWrite(OUT2, 0);
  digitalWrite(OUT3, 0);

  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  keypad.addEventListener(keypadEvent);
  lcdDefaultInterface();
}

void loop()
{
  // Serial.println(digitalRead(RF1));
  // Serial.println(digitalRead(RF2));
  // Serial.println(digitalRead(RF3));
  delay(1000);
  // on/off relay with remote controller
  if (digitalRead(RF1) & !flagEnRelay1)
  {
    digitalWrite(OUT1, 1);
    flagEnRelay1 = true;
    lastDuration1 = t.min;
  }
  else if (digitalRead(RF2) && !flagEnRelay2)
  {
    digitalWrite(OUT2, 1);
    flagEnRelay2 = true;
    lastDuration2 = t.min;
  }
  else if (digitalRead(RF3) && !flagEnRelay3)
  {
    digitalWrite(OUT3, 1);
    flagEnRelay3 = true;
    lastDuration3 = t.min;
  }

  // hold relay in "compareDuration" minnutes
  if (flagEnRelay1)
  {
    if ((t.min - lastDuration1) >= compareDuration1)
    {
      digitalWrite(OUT1, 0);
      compareDuration1 = 5; // set to default
      flagEnRelay1 = false;
    }
  }
  if (flagEnRelay2)
    if ((t.min - lastDuration2) >= compareDuration2)
    {
      digitalWrite(OUT2, 0);
      compareDuration2 = 5; // set to default
      flagEnRelay2 = false;
    }
  if (flagEnRelay3)
    if ((t.min - lastDuration3) >= compareDuration3)
    {
      digitalWrite(OUT3, 0);
      compareDuration3 = 5; // set to default
      flagEnRelay3 = false;
    }

  // loop get time
  DS3231_get(&t);

  char key = keypad.getKey();

  if (!flagCusSetting && !flagRepeatSetting && !flagCusView && !flagRepeatView)
    lcdHomeScreen();

  // check EEPROM value every 1 minutes
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
    if ((key == 'A') && !flagRepeatSetting && !flagCusSetting && !flagCusView)
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