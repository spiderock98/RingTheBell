int LAMP = 6;

int dimVal = 0;
int adcVal = 0;
int now = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(LAMP, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(3), zeroCross, FALLING);
}
void zeroCross()
{
  digitalWrite(LAMP, LOW);

  delayMicroseconds(200 * dim_val);

  digitalWrite(LAMP, HIGH);
}

void loop()

{
  //now = millis();
  // adcVal = analogRead(A1);
  if (millis() - now >= 5000)
  {
    dimVal += 10;
    now = millis();
  }

  //Serial.println(adcVal);

  //dim_val = map(adcVal, 0, 1023, 10, 49);
}
