#include "misc.h"
#include "repeat.h"
#include "custom.h"

//================================================== global variables ==================================================
struct ts t;
volatile int16_t iAddressEEProm = -7, iCusAddressEEProm = 187;
volatile int8_t chosenDayOfWeek = -1;
byte arrTick[1024], iCusEvents = EEPROM.read(196);

bool flagRepeatSetting = false, flagCusSetting = false, flagCusView = false, flagRepeatView = false, flagEnRelay1 = false, flagEnRelay2 = false, flagEnRelay3 = false, flagTickMinus1 = false, flagTickMinus2 = false, flagTickMinus3 = false, flagSetRTC = false, flagSetRfDuration = false, flagPulsePerHour = false;

bool lastPulseState = false;

int8_t lastMinAlarm = -1, lastMinBacklight = 0;
int8_t lastDuration1 = 0, lastDuration2 = 0, lastDuration3 = 0;
uint8_t compareDuration1 = 0, compareDuration2 = 0, compareDuration3 = 0; // minutes, init later in setup()
int8_t lastSecPulse, lastHourPulse;
uint8_t countPulsePerHour;                                   // number of pulse every 1 hour = t.hour
uint8_t PulseWidthPerHourHIGH = 1, PulseWidthPerHourLOW = 2; // <sec> ON <sec> OFF

//================================================== setup ==================================================

void setup()
{
  // Serial.begin(9600);

  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(OUT4, OUTPUT); // pulse per hour
  pinMode(OUT5, OUTPUT); // relay mass wire
  digitalWrite(OUT1, 0);
  digitalWrite(OUT2, 0);
  digitalWrite(OUT3, 0);
  digitalWrite(OUT4, 0);
  digitalWrite(OUT5, 0); // relay OFF mass wire

  compareDuration1 = EEPROM.read(1025);
  compareDuration2 = EEPROM.read(1026);
  compareDuration3 = EEPROM.read(1027);

  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  keypad.addEventListener(keypadEvent);
  WelcomeInterface();

  DS3231_get(&t);
  lastMinAlarm = t.min;
  lastHourPulse = t.hour;
}

//================================================== Loop ==================================================

void loop()
{
#pragma region on / off relay with remote controller
  // off all relay no condition
  if (analogRead(RF0) > 612) // 3V
  {
    digitalWrite(OUT1, 0);
    digitalWrite(OUT2, 0);
    digitalWrite(OUT3, 0);
    digitalWrite(OUT5, 1); // relay ON mass wire
    // set to default for next RF control
    compareDuration1 = EEPROM.read(1025);
    compareDuration2 = EEPROM.read(1026);
    compareDuration3 = EEPROM.read(1027);
    flagEnRelay1 = flagEnRelay2 = flagEnRelay3 = flagTickMinus1 = flagTickMinus2 = flagTickMinus3 = false;
  }

  if ((analogRead(RF1) > 612) && !flagEnRelay1)
  {
    digitalWrite(OUT1, 1);
    digitalWrite(OUT5, 1); // relay ON mass wire
    flagEnRelay1 = true;
    lastDuration1 = t.min; // begin timer
  }
  else if ((analogRead(RF2) > 612) && !flagEnRelay2)
  {
    digitalWrite(OUT2, 1);
    digitalWrite(OUT5, 1); // relay ON mass wire
    flagEnRelay2 = true;
    lastDuration2 = t.min; // begin timer
  }
  else if ((analogRead(RF3) > 612) && !flagEnRelay3)
  {
    digitalWrite(OUT3, 1);
    digitalWrite(OUT5, 1); // relay ON mass wire
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
      digitalWrite(OUT1, 0);                // then turn off relay
      compareDuration1 = EEPROM.read(1025); // set to default for next RF control
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
      compareDuration2 = EEPROM.read(1026);
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
      compareDuration3 = EEPROM.read(1027);
      flagEnRelay3 = flagTickMinus3 = false;
    }
  }

  // Ex: 18:59 next is 18:00 -> buggy
  if ((t.min == 59) && (t.sec >= 59))
  {
    lastMinAlarm = -1;
    delay(2000); // nếu ko block >3s ở đây else if phía dưới sẽ bắt kịp và set lastMinAlarm về 59
  }
  // check EEPROM value every 1 minutes for turn ON relays, not in phút 59
  else if (t.min - lastMinAlarm)
  {
    alarm();
    lastMinAlarm = t.min;
    lcd.noBacklight(); // off screen after 1 min no activity
    if (!flagEnRelay1 && !flagEnRelay2 && !flagEnRelay3)
      digitalWrite(OUT5, 0); // check relay OFF mass wire every 1 min
  }

  // init these value after 1 hours
  if (t.hour - lastHourPulse)
  {
    flagPulsePerHour = true;
    lastHourPulse = t.hour;
    lastSecPulse = t.sec; // init second >> nearly 0
    if (t.hour > 21)      // from 22h00
    {
      countPulsePerHour = 2; // nightly >> 1 pulse
    }
    else if (t.hour > 12) // from 1 P.M
    {
      countPulsePerHour = (t.hour - 12) * 2;
    }
    else if (t.hour > 0) // from 1 A.M
    {
      countPulsePerHour = t.hour * 2;
    }
    else // 0h00 A.M -> 3 pulse
    {
      countPulsePerHour = 3 * 2;
    }
  }
  // pulse per hour func()
  if (flagPulsePerHour)
  {
    if (t.sec == 0)
    {
      lastSecPulse = 0;
    }
    if (((t.sec - lastSecPulse) >= PulseWidthPerHourHIGH) && lastPulseState)
    {
      lastSecPulse = t.sec;
      if (countPulsePerHour--)
      {
        digitalWrite(OUT4, LOW); // off
        lastPulseState = !lastPulseState;
      }
      else
        flagPulsePerHour = false;
    }
    if (((t.sec - lastSecPulse) >= PulseWidthPerHourLOW) && !lastPulseState)
    {
      lastSecPulse = t.sec;
      if (countPulsePerHour--)
      {
        digitalWrite(OUT4, HIGH); // on
        lastPulseState = !lastPulseState;
      }
      else
        flagPulsePerHour = false;
    }
  }

  // loop home screen to update RTC
  if (!flagCusSetting && !flagRepeatSetting && !flagCusView && !flagRepeatView && !flagSetRTC && !flagSetRfDuration)
    lcdHomeScreen();

  // loop get time
  DS3231_get(&t);
  char key = keypad.getKey();
}

//================================================== Keypad Event Handler ==================================================

void keypadEvent(KeypadEvent key)
{
  lcd.backlight(); // ON screen when pressed

  switch (keypad.getState())
  {
  case PRESSED:
    if ((key == 'A') && !flagRepeatSetting && !flagCusSetting && !flagCusView && !flagSetRTC && !flagSetRfDuration)
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
    else if ((key == 'D') && !flagRepeatView && !flagCusSetting && !flagSetRTC && !flagSetRfDuration)
    {
      if (EEPROM.read(196) == 0)
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
      } while ((EEPROM.read(iCusAddressEEProm) == 0) && (iCusAddressEEProm < 1017) && iCusEvents);

      if (!iCusEvents)
      {
        iCusEvents = EEPROM.read(196);
        iCusAddressEEProm = 187;
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
      if (!flagCusSetting && !flagRepeatSetting && !flagRepeatView && !flagSetRTC && !flagSetRfDuration) // just in view can add event
      {
        // fix when hold d >> you must pass PRESSED D and get these stuff wrong
        iCusAddressEEProm = 187;
        ++iCusEvents;
        flagCusView = false;

        do
        { // find null event >> avoid EEProm diskfrag
          iCusAddressEEProm += 10;
        } while ((EEPROM.read(iCusAddressEEProm) != 0) && (iCusAddressEEProm < 1017));

        customSetValue();
      }
    }

    else if ((key == '#'))
    { // xoá sự kiện custom
      if (flagCusView)
      {
        customDeleteValue();
        flagCusView = false;
      }
      // set duration pressed rf
      else
      {
        setRfDuration();
      }
    }

    // set ds3231 rtc
    else if (key == '*')
    {
      if (!flagSetRTC)
      {
        setRTC();
        lastHourPulse = t.hour; // fix pulse after set rtc
      }
    }

    break;
  }
}