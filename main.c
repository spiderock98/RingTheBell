#include <main.h>
#include "def_lcd.c"

#define triac1 RB7
#define triac2 RB6
#define relay RB5

signed int32 count = 0;
int1 flag = true;

#INT_EXT
void ext_isr()
{
   if (RB1)
   {
      //clockwise
      ++count;
   }
   else
   {
      --count;
   }
   clear_interrupt(INT_EXT);
}

#INT_CCP1
void ccp1_isr()
{
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
   triac1 = 1;
   triac2 = 0;
}
void REVERSE()
{
   triac1 = 0;
   triac2 = 1;
}
void STOP()
{
   triac1 = 1;
   triac2 = 1;
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

   clear_interrupt(INT_EXT);
   enable_interrupts(INT_EXT);
   ext_int_edge(H_TO_L);

   setup_ccp1(CCP_CAPTURE_FE);
   setup_timer_1(T1_INTERNAL);
   enable_interrupts(INT_CCP1);

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
