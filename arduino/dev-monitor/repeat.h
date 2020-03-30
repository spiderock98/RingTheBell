#include <Arduino.h>
#include <EEPROM.h>
#include "misc.h"

void initRepeaterInterface();
void repeaterSetValue();
void repeaterInterface();
enum addressCol
{
    TimeInDay = 0,
    DayOfWeek = 6,
    HOUR = 0,
    MIN = 3,
    RELAY1 = 9,
    RELAY2 = 12,
    RELAY3 = 15,
    DURATION = 10
};