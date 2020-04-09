
int count = 0;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  //pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(3), ngatngoai, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  //  if (digitalRead(3)){
  //    if (
  //  }
  //Serial.println(count);
}

void ngatngoai()
{
  delayMicroseconds(20);
  noInterrupts();
  if (digitalRead(4))
  {
    //count++;
    Serial.println(++count);
  }
  else
  {
    //count--;
    Serial.println(--count);
  }
  interrupts();
}
