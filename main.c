#include <main.h>
#include "def_lcd.c"
#include "floor.c"

#define triac1 RB7
#define triac2 RB6
#define relay RB5

// set angle (10 levels). more angle, weaker motor
float32 angleStarter = 1;
float32 angleRingTheBell = 6;

int32 valTimer0SetStarter = (int32)FLOOR((13.1072 - angleStarter) / 0.0512) - 1;
int32 valTimer0SetRingTheBell = (int32)FLOOR((13.1072 - angleRingTheBell) / 0.0512) - 1;

signed int32 count = 0;
int8 countTime = 0;
int1 flagForward = true;
int1 flagStarter = true;
int flagSTOP = false;
int8 anglePercent = 0;

#INT_EXT
void ext_isr()
{
   // cross zero point detector 50Hz >> 1/50/2 sec per half pulse after diode brighet

   if (flagForward)
      triac1 = 0;
   else
      triac2 = 0;

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
   // ++countTime;
   // if (countTime == 100)
   // {
   //    countTime = 0;
   //    //set_timer0((int32)59.00);
   //    PORTA = 0xFF;
   //    delay_ms(500);
   //    PORTA = 0;
   // }
   if (flagForward && !flagSTOP)
      triac1 = 1;
   else if (!flagForward && !flagSTOP)
      triac2 = 1;

   // set_timer0(200.00);
   disable_interrupts(INT_TIMER0);
}

#INT_CCP1
void ccp1_isr()
{
   // encoder
   if (RB1)
   {
      //clockwise
      ++count;
   }
   else
   {
      --count;
   }
}

void FORWARD()
{
   triac2 = 0;
   flagForward = true;
   flagSTOP = false;
}
void REVERSE()
{
   triac1 = 0;
   flagForward = flagSTOP = false;
}
void STOP()
{
   flagSTOP = true;
   triac1 = triac2 = 0;
}

void starter()
{
   while (count <= 400)
      FORWARD();
   STOP();

   ghima(0x01); // clear
   ghima(0x80); // set 0,0
   hienthi(arrNumber[count / 10]);
   hienthi(arrNumber[count % 10]);

   while (count >= 0)
      STOP();

   ghima(0x01); // clear
   ghima(0x80); // set 0,0
   hienthi(arrNumber[count / 10]);
   hienthi(arrNumber[count % 10]);

   while (count >= -400)
      REVERSE();
   STOP();

   ghima(0x01); // clear
   ghima(0x80); // set 0,0
   hienthi(arrNumber[count / 10]);
   hienthi(arrNumber[count % 10]);

   while (count <= 0)
      STOP();

   ghima(0x01); // clear
   ghima(0x80); // set 0,0
   hienthi(arrNumber[count / 10]);
   hienthi(arrNumber[count % 10]);

   while (count <= 800)
      FORWARD();
   STOP();

   ghima(0x01); // clear
   ghima(0x80); // set 0,0
   hienthi(arrNumber[count / 10]);
   hienthi(arrNumber[count % 10]);

   while (count >= 0)
      STOP();

   ghima(0x01); // clear
   ghima(0x80); // set 0,0
   hienthi(arrNumber[count / 10]);
   hienthi(arrNumber[count % 10]);

   while (count >= -800)
      REVERSE();
   STOP();

   ghima(0x01); // clear
   ghima(0x80); // set 0,0
   hienthi(arrNumber[count / 10]);
   hienthi(arrNumber[count % 10]);

   while (count <= 0)
      STOP();

   ghima(0x01); // clear
   ghima(0x80); // set 0,0
   hienthi(arrNumber[count / 10]);
   hienthi(arrNumber[count % 10]);

   while (count <= 1200)
      FORWARD();
   STOP();

   ghima(0x01); // clear
   ghima(0x80); // set 0,0
   hienthi(arrNumber[count / 10]);
   hienthi(arrNumber[count % 10]);

   flagStarter = false;
}

void RingTheBell()
{
   while (count >= 400)
      STOP();

   ghima(0x01); // clear
   ghima(0x80); // set 0,0
   hienthi(arrNumber[count / 10]);
   hienthi(arrNumber[count % 10]);

   while (count >= -800)
      REVERSE();
   STOP();

   ghima(0x01); // clear
   ghima(0x80); // set 0,0
   hienthi(arrNumber[count / 10]);
   hienthi(arrNumber[count % 10]);

   while (count <= -400)
      STOP();

   ghima(0x01); // clear
   ghima(0x80); // set 0,0
   hienthi(arrNumber[count / 10]);
   hienthi(arrNumber[count % 10]);

   while (count <= 800)
      FORWARD();
   STOP();

   ghima(0x01); // clear
   ghima(0x80); // set 0,0
   hienthi(arrNumber[count / 10]);
   hienthi(arrNumber[count % 10]);
}

void main()
{
   initLCD();

   ghima(0x01); // clear
   ghima(0x80); // set 0,0
   hienthi(arrNumber[count / 10]);
   hienthi(arrNumber[count % 10]);

   TRISB0 = TRISB1 = TRISC2 = 1; //input
   TRISB5 = TRISB6 = TRISB7 = 0; //output
   TRISA = 0;

   clear_interrupt(INT_EXT);
   enable_interrupts(INT_EXT);
   ext_int_edge(H_TO_L);

   setup_ccp1(CCP_CAPTURE_FE);
   setup_timer_1(T1_INTERNAL);
   enable_interrupts(INT_CCP1);

   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256); //51.2us
   set_timer0(200);                             //10.086ms
   enable_interrupts(INT_TIMER0);

   enable_interrupts(GLOBAL);

   // PORT_B_PULLUPS(0xff);

   while (TRUE)
   {
      // ghima(0x80); // set 0,0
      // hienthi(arrNumber[count / 10]);
      // hienthi(arrNumber[count % 10]);
      // if (flag)
      // {
      //    RB2 = RB3 = RB4 = 1;
      // }
      // else
      // {
      //    RB2 = RB3 = RB4 = 0;
      // }
   }
}
