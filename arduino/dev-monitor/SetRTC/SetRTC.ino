#include <Wire.h>
#include <ds3231.h>

struct ts t;

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    DS3231_init(DS3231_CONTROL_INTCN);
    /*----------------------------------------------------------------------------
    In order to synchronise your clock module, insert timetable values below !
    ----------------------------------------------------------------------------*/
    t.hour = 18;
    t.min = 43;
    t.sec = 30;
    t.mday = 31;
    t.mon = 3;
    t.year = 2020;
    t.wday = 3;

    DS3231_set(t);
}

void loop()
{
    DS3231_get(&t);
    Serial.print("Wday : ");
    Serial.print(t.wday);
    Serial.print("\t Date : ");
    Serial.print(t.mday);
    Serial.print("/");
    Serial.print(t.mon);
    Serial.print("/");
    Serial.print(t.year);
    Serial.print("\t Hour : ");
    Serial.print(t.hour);
    Serial.print(":");
    Serial.print(t.min);
    Serial.print(".");
    Serial.println(t.sec);

    delay(1000);
}
