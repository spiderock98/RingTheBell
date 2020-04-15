#include <Streaming.h>

void checkSafetyFirst(int32_t sec);

volatile int16_t count;

void setup()
{
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), blink, FALLING);

  checkSafetyFirst(10000000);
}

void blink()
{
  noInterrupts();
  if (digitalRead(3))
    ++count;
  else
    --count;
  interrupts();
}

void loop()
{
  //Serial << count << endl;
}

void checkSafetyFirst(int32_t sec)
{
  Serial << "begining check saftefy" << endl;
  int16_t lastCount = count;
  for (int32_t i = sec; --i;) // waitting steady
  {
    if (count != lastCount)
    {
      i = sec; // reset
      lastCount = count;
      //delayMicroseconds(20); // pray for counting up
      delay(30);
    }
  }
  count = 0; // update 0 point
  Serial << "Done Safety: " << count << endl;
}
