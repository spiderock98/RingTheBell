#include <Keypad.h>
#include <Wire.h>
#include <ds3231.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

struct ts t;

LiquidCrystal_I2C lcd(0x3F, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

uint8_t bell[8]  = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
uint8_t clock[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {6, 7, 8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 3, 4, 5}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void lcdDefaultInterface() {
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, bell);
  lcd.createChar(1, clock);

  lcd.setCursor(0, 0);
  lcd.print("AUTO CHURCH BELL");
  lcd.setCursor(3, 1);
  for (byte i = 0; 5 - i;) {
    lcd.printByte(0);
    delay(200);
    lcd.printByte(1);
    delay(200);
    ++i;
  }
  lcd.noBacklight();
  lcd.clear();
}

void setup() {
  Serial.begin(9600);

  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);

  lcdDefaultInterface();

  keypad.addEventListener(keypadEvent);
}

void loop() {
  DS3231_get(&t);
  char key = keypad.getKey();

  if (key) {
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print((int)key - 48);
  }

  lcd.setCursor(0, 1);
  lcd.print(t.hour);
  lcd.print(":");
  lcd.print(t.min);
  lcd.print(":");
  lcd.print(t.sec);
  
}
void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    //    case PRESSED:
    //      if (key == '#') {
    //        digitalWrite(ledPin, !digitalRead(ledPin));
    //        ledPin_state = digitalRead(ledPin);        // Remember LED state, lit or unlit.
    //      }
    //      break;

    //    case RELEASED:
    //      if (key == '*') {
    //        digitalWrite(ledPin, ledPin_state);   // Restore LED state from before it started blinking.
    //        blink = false;
    //      }
    //      break;

    case HOLD:
      if (key == '*') {
        lcd.clear();
      }
      break;
  }
}
