#include <main.h>
#include "def_lcd.c"
#include "floor.c"

#define triac1 RC6
#define triac1Out PIN_C6

#define triac2 RC7
#define triac2Out PIN_C7

#define relay RC1
#define relayOut PIN_C1

// set angle (10 levels). more angle, weaker motor
float32 angleStarter = 1;
float32 angleRingTheBell = 6;

int32 valTimer0SetStarter = (int32)FLOOR((13.1072 - angleStarter) / 0.0512) - 1;
int32 valTimer0SetRingTheBell = (int32)FLOOR((13.1072 - angleRingTheBell) / 0.0512) - 1;

signed int32 count = 0;
int8 countTime = 0;
int1 flagForward = true;
int1 flagStarter = true;
int1 flagSTOP = false;
int8 anglePercent = 0;

#INT_EXT
void ext_isr()
{
   // cross zero point detector 50Hz >> 1/50/2 sec per half pulse after diode brighet

   // if (flagForward)
   //    triac1 = 0;
   // else
   //    triac2 = 0;

   output_low(triac1Out);
   output_low(triac2Out);

   if (flagStarter)
      set_timer0(valTimer0SetStarter);
   else
      set_timer0(valTimer0SetRingTheBell);

   clear_interrupt(INT_TIMER0);
   enable_interrupts(INT_TIMER0);
   clear_interrupt(INT_EXT); // waiting for next EXT
}

#INT_TIMER0
void timer0_isr()
{
   if (flagForward && !flagSTOP)
      output_high(triac1Out);
   else if (!flagForward && !flagSTOP)
      output_high(triac2Out);

   // set_timer0(200.00);
   disable_interrupts(INT_TIMER0);
}

#INT_CCP1
void ccp1_isr()
{
   if (RC0)
      ++count;
   else
      --count;
   // clear_interrupt(INT_CCP1);
}

void FORWARD()
{
   output_high(relayOut);
   output_low(triac2Out);

   flagForward = true;
   flagSTOP = false;
}
void REVERSE()
{
   output_high(relayOut);
   output_low(triac1Out);
   flagForward = flagSTOP = false;
}
void STOP()
{
   output_low(relayOut);
   flagSTOP = true;
   output_low(triac1Out);
   output_low(triac2Out);
}

void starter()
{
   while (count <= 400)
      FORWARD();
   STOP();

   while (count >= 0)
      STOP();

   while (count >= -400)
      REVERSE();
   STOP();

   while (count <= 0)
      STOP();

   while (count <= 800)
      FORWARD();
   STOP();

   while (count >= 0)
      STOP();

   while (count >= -800)
      REVERSE();
   STOP();

   while (count <= 0)
      STOP();

   while (count <= 1200)
      FORWARD();
   STOP();

   flagStarter = false;
}

void RingTheBell()
{
   while (count >= 400)
      STOP();

   while (count >= -800)
      REVERSE();
   STOP();

   while (count <= -400)
      STOP();

   while (count <= 800)
      FORWARD();
   STOP();
}

void main()
{
   // initLCD();

   // ghima(0x01); // clear
   // ghima(0x80); // set 0,0
   // hienthi(arrNumber[count / 10]);
   // hienthi(arrNumber[count % 10]);

   TRISB0 = TRISC0 = TRISC2 = 1; //input
   TRISC6 = TRISC7 = TRISC1 = 0; //output

   clear_interrupt(INT_EXT);
   enable_interrupts(INT_EXT);
   ext_int_edge(H_TO_L);

   setup_ccp1(CCP_CAPTURE_FE);
   setup_timer_1(T1_INTERNAL);
   enable_interrupts(INT_CCP1);

   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256); //51.2us
   // set_timer0(59);                             //10.086ms
   enable_interrupts(INT_TIMER0);

   enable_interrupts(GLOBAL);

   starter();

   // PORT_B_PULLUPS(0xff);

   while (TRUE)
   {
      RingTheBell();
   }
}
