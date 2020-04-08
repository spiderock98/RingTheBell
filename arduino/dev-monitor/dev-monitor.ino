#include "misc.h"
#include "repeat.h"
#include "custom.h"

struct ts t;
volatile int16_t chosenDayOfWeek = -1, iAddressEEProm = -7, iCusAddressEEProm = 138, numOfEvents = 0;
byte arrTick[256], iCusEvents = EEPROM[147];

bool flagRepeatSetting = false, flagCusSetting = false, flagCusView = false, flagRepeatView = false, flagEnRelay1 = false, flagEnRelay2 = false, flagEnRelay3 = false, flagTickMinus1 = false, flagTickMinus2 = false, flagTickMinus3 = false, flagSetRTC = false;

int8_t lastMinAlarm = -1, lastMinBacklight = 0;
int8_t lastDuration1 = 0, lastDuration2 = 0, lastDuration3 = 0;
uint8_t compareDuration1 = 5, compareDuration2 = 5, compareDuration3 = 5; //minutes
uint8_t timeBeforeTick0;

void setup()
{
  Serial.begin(9600);

  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  digitalWrite(OUT1, 0);
  digitalWrite(OUT2, 0);
  digitalWrite(OUT3, 0);

  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  keypad.addEventListener(keypadEvent);
  WelcomeInterface();
}

void loop()
{
#pragma region on / off relay with remote controller
  // off all relay no condition
  if (analogRead(RF0) > 612) //3V
  {
    digitalWrite(OUT1, 0);
    digitalWrite(OUT2, 0);
    digitalWrite(OUT3, 0);
    compareDuration1 = compareDuration2 = compareDuration3 = defaultDuration; // set to default for next RF control
    flagEnRelay1 = flagEnRelay2 = flagEnRelay3 = flagTickMinus1 = flagTickMinus2 = flagTickMinus3 = false;
  }

  if ((analogRead(RF1) > 612) && !flagEnRelay1)
  {
    digitalWrite(OUT1, 1);
    flagEnRelay1 = true;
    lastDuration1 = t.min; // begin timer
  }
  else if ((analogRead(RF2) > 612) && !flagEnRelay2)
  {
    digitalWrite(OUT2, 1);
    flagEnRelay2 = true;
    lastDuration2 = t.min; // begin timer
  }
  else if ((analogRead(RF3) > 612) && !flagEnRelay3)
  {
    digitalWrite(OUT3, 1);
    flagEnRelay3 = true;
    lastDuration3 = t.min; // begin timer
  }
#pragma endregion

  // hold relay in <compareDuration> minutes
  if (flagEnRelay1)
  {
    if ((t.min == 0) && (lastDuration1 != 0) && !flagTickMinus1)
    {
      lastDuration1 = lastDuration1 - 60;
      flagTickMinus1 = true;
    }
    if ((t.min - lastDuration1) >= (compareDuration1))
    {
      digitalWrite(OUT1, 0);              // then turn off relay
      compareDuration1 = defaultDuration; // set to default for next RF control
      flagEnRelay1 = flagTickMinus1 = false;
    }
  }
  if (flagEnRelay2)
  {
    if ((t.min == 0) && (lastDuration2 != 0) && !flagTickMinus2)
    {
      lastDuration2 = lastDuration2 - 60;
      flagTickMinus2 = true;
    }
    if ((t.min - lastDuration2) >= compareDuration2)
    {
      digitalWrite(OUT2, 0);
      compareDuration2 = defaultDuration;
      flagEnRelay2 = flagTickMinus2 = false;
    }
  }

  if (flagEnRelay3)
  {
    if ((t.min == 0) && (lastDuration3 != 0) && !flagTickMinus3)
    {
      lastDuration3 = lastDuration3 - 60;
      flagTickMinus3 = true;
    }
    if ((t.min - lastDuration3) >= compareDuration3)
    {
      digitalWrite(OUT3, 0);
      compareDuration3 = defaultDuration;
      flagEnRelay3 = flagTickMinus3 = false;
    }
  }

  // loop get time
  DS3231_get(&t);

  char key = keypad.getKey();

  // loop home screen to update RTC
  if (!flagCusSetting && !flagRepeatSetting && !flagCusView && !flagRepeatView && !flagSetRTC)
  {
    lcdHomeScreen();
  }

  // Ex: 18:59 next is 18:00 -> buggy
  if ((t.min == 59) && (t.sec >= 58))
  {
    lastMinAlarm = -1;
    delay(3000); // nếu ko block >3s ở đây else if phía dưới sẽ bắt kịp và set lastMinAlarm về 59
  }
  // check EEPROM value every 1 minutes for alarm(), not in phút 59
  else if ((t.min - lastMinAlarm))
  {
    // if (t.min == 59)
    //   flagTick59 = true;
    alarm();
    lastMinAlarm = t.min;
  }

  // off screen after 10 second no activity
  else if (t.min - lastMinBacklight)
    lcd.noBacklight();
}
// software interrupt event keypad
void keypadEvent(KeypadEvent key)
{
  lcd.backlight();          // ON screen when pressed
  lastMinBacklight = t.min; // update time stamp

  switch (keypad.getState())
  {
  case PRESSED:
    if ((key == 'A') && !flagRepeatSetting && !flagCusSetting && !flagCusView)
    {
      ++chosenDayOfWeek;
      iAddressEEProm += 7;
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
        // return home screen
        lcd.clear();
        return;
      }
      do
      { // pass null event to generate iCusAddressEEProm
        iCusAddressEEProm += 10;
        // Serial.println();
        // Serial.print("iCusAddressEEProm: ");
        // Serial.println(iCusAddressEEProm);
        // Serial.println(iCusEvents);
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
    // xoá sự kiện custom
    else if ((key == '#') && flagCusView)
    {
      customDeleteValue();
      flagCusView = false;
    }

    // set ds3231
    else if (key == '*')
    {
      if (!flagSetRTC)
        setRTC();
      else
      {
        flagSetRTC = false;
        lcd.clear();
      }
    }

    break;
  }
}