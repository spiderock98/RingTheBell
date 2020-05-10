
#define initFeedbackCount 5

volatile int count = 0;
volatile bool flagFbForward = 1;
volatile int8_t countFbForward = initFeedbackCount, countFbReverse = initFeedbackCount;

void setup()
{
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  //pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(3), ngatngoai, RISING);
}

void loop()
{
  // put your main code here, to run repeatedly:
  //  if (digitalRead(3)){
  //    if (
  //  }
  //Serial.println(count);

  for (int16_t i = 5;; i += 5)
  {
    while (count <= i)
      FORWARD();
    STOP();
    while (count >= i / 2)
      STOP();
    while (count >= -i)
      REVERSE();
    STOP();
    while (count <= -i / 2)
      STOP();
  }
}

void FORWARD()
{
  Serial.println("quay thuan");
  if (!flagFbForward) // suddendly reverse for n pulses
  {
    delay(500);
    if (!(--countFbReverse))
    {
      reset_cpu();
    }
    return;
  }
  countFbReverse = initFeedbackCount; // ready for next change dir
  countFbForward = initFeedbackCount;
  delay(500);
}
void REVERSE()
{
  Serial.println("quay nghich");
  if (flagFbForward) // suddendly forward for 10 pulses
  {
    delay(500);
    if (!(--countFbForward))
    {
      reset_cpu();
    }
    return;
  }
  countFbForward = initFeedbackCount;
  countFbReverse = initFeedbackCount; // ready for next change dir
  delay(500);
}
void STOP()
{
  Serial.println("dung");
  delay(500);
}
void ngatngoai()
{
  noInterrupts();
  if (digitalRead(4))
  {
    // count++;
    flagFbForward = 1;
    Serial.println(++count);
  }
  else
  {
    // count--;
    flagFbForward = 0;
    Serial.println(--count);
  }
  interrupts();
}
void reset_cpu()
{
  Serial.println("!!!!reset cpu!!!!");
}
