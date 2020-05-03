
int count = 0;

void setup() {
  Serial.begin(115200);
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

void FORWARD() {
  Serial.println("quay thuan");
  delay(500);
}
void REVERSE() {
  Serial.println("quay nghich");
  delay(500);
}
void STOP() {
  Serial.println("dung");
  delay(500);
}
void ngatngoai()
{
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
