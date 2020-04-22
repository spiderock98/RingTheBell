#include <EEPROM.h>

void setup()
{
  // put your setup code here, to run once:
  for (int i = 0; i < EEPROM.length(); i++)
  {
    EEPROM.write(i, 0);
    delay(4);
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
}