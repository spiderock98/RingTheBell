#include <Arduino.h>
#include "misc.h"

void customSetValue();
void customInterface();
void blankCusInterface();
void _blankCusInterface();
void customDeleteValue();

enum cusAddressCol
{
    cusDayOfMonth = 0,
    cusMONTH = 3,
    cusYEAR1 = 6,
    cusYEAR2 = 8,
    cusHOUR = 11,
    cusMIN = 14,
    cusRELAY1 = 1,
    cusRELAY2 = 4,
    cusRELAY3 = 7,
    cusDURATION = 13
};