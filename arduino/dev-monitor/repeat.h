#include <Arduino.h>
#include "misc.h"

void EEPr2DailyUI();
void repeaterSetValue();
void repeaterInterface();

// addr of the collums of some ticks >> for quick print lcd
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