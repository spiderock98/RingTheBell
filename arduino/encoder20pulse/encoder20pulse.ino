
int count = 0;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  //pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), ISR_EXT, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void ISR_EXT()
{
  noInterrupts();
  if (digitalRead(3))
  {
    //Serial.print('+');
    Serial.println(++count);
  }
  else
  {
    //Serial.print('-');
    Serial.println(--count);
  }
  interrupts();
}
