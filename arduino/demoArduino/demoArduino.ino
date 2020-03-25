#define relay 8
#define triac1 5
#define triac2 6

int count = 0;
bool flag = true;

void thuan()
{
  digitalWrite(triac1, 1);
  digitalWrite(triac2, 0);
  //Serial.println("THUAN");
}
void nghich()
{
  digitalWrite(triac1, 0);
  digitalWrite(triac2, 1);
  //Serial.println("NGHICH");
}
void dung()
{
  digitalWrite(triac1, 0);
  digitalWrite(triac2, 0);
  //Serial.println("STOP");
}
void khoidong()
{
  while (count <= 400)
    thuan();
  while (count >= 0)
    dung();
  while (count >= -400)
    nghich();
  while (count <= 0)
    dung();
  while (count <= 800)
    thuan();
  while (count >= 0)
    dung();
  while (count >= -800)
    nghich();
  while (count <= 0)
    dung();
  while (count <= 1200)
    thuan();
  dung();
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(relay, OUTPUT);
  pinMode(triac1, OUTPUT);
  pinMode(triac2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), ISR_EXT, FALLING);
  delay(500);
  khoidong();
}

void ISR_EXT()
{
  if (digitalRead(3))
  {
    ++count;
    flag = true;
    //    if (count >= 1200)
    //      flag = false;
    //    else
    //      flag = true;
  }
  else
  {
    --count;
    flag = false;
  }
}

void keochuong()
{
  while (count >= 400)
    dung();
  while (count >= -800)
    nghich();
  while (count <= -400)
    dung();
  while (count <= 800)
    thuan();
}

void loop()
{
  // put your main code here, to run repeatedly:
  keochuong();
  //  if (flag)
  //    Serial.println("PULL");
  //  else
  //    Serial.println("DROP");
}
