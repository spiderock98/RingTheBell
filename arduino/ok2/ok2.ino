#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

//#define relay 8
#define triac1 5
#define triac2 6

long count = 0;
//bool flag = true;

void thuan()
{
  digitalWrite(LED_BUILTIN, 1);
  // lcd.setCursor(0, 0);
  // lcd.print(count);
  digitalWrite(triac1, 1);
  digitalWrite(triac2, 0);
  //Serial.println("THUAN");
}
void nghich()
{
  digitalWrite(LED_BUILTIN, 1);
  // lcd.setCursor(0, 0);
  // lcd.print(count);
  digitalWrite(triac1, 0);
  digitalWrite(triac2, 1);
  //Serial.println("NGHICH");
}
void dung()
{
  digitalWrite(LED_BUILTIN, 0);
  // lcd.setCursor(0, 0);
  // lcd.print(count);
  digitalWrite(triac1, 0);
  digitalWrite(triac2, 0);
  //Serial.println("STOP");
}
void khoidong()
{
  while (count <= 400)
    thuan();
  dung();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(count);
  while (count >= 0)
    dung();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(count);
  while (count >= -400)
    nghich();
  dung();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(count);
  while (count <= 0)
    dung();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(count);
  while (count <= 800)
    thuan();
  dung();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(count);
  while (count >= 0)
    dung();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(count);
  while (count >= -800)
    nghich();
  dung();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(count);
  while (count <= 0)
    dung();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(count);
  while (count <= 1200)
    thuan();
  dung();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(count);
}

void setup()
{
  //Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(count);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  //pinMode(relay, OUTPUT);
  pinMode(triac1, OUTPUT);
  pinMode(triac2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), ISR_EXT, FALLING);

  khoidong(); //loop here
}

void ISR_EXT()
{
  if (digitalRead(3))
  {
    ++count;
    // digitalWrite(LED_BUILTIN, 1);
  }
  else
  {
    --count;
    // digitalWrite(LED_BUILTIN, 0);
  }
}

void keochuong()
{
  while (count >= 400)
    dung();
  lcd.setCursor(0, 0);
  lcd.print(count);
  while (count >= -800)
    nghich();
  dung();
  lcd.setCursor(0, 0);
  lcd.print(count);
  while (count <= -400)
    dung();
  lcd.setCursor(0, 0);
  lcd.print(count);
  while (count <= 800)
    thuan();
  dung();
  lcd.setCursor(0, 0);
  lcd.print(count);
}

void loop()
{
  keochuong();
  //lcd.setCursor(0,0);
  //lcd.print(count);
  //  if (flag)
  //    Serial.println("PULL");
  //  else
  //    Serial.println("DROP");
}
