#include <main.h>
#include "def_lcd.c"

// #define triac1 5
// #define triac2 6

signed int32 count = 0;
int1 flag = true;

// #INT_EXT
// void ext_isr()
// {
//    if (RB1)
//    {
//       //clockwise
//       ++count;
//       // if (count >= 100)
//       //    flag = false;
//       // else
//       //    flag = true;
//       RC2 = 1;
//    }
//    else
//    {
//       --count;
//       RC2 = 0;
//       // flag = false;
//    }
//    clear_interrupt(INT_EXT);
// }

// void thuan()
// {
//    digitalWrite(triac1, 1);
//    digitalWrite(triac2, 0);
//    Serial.println("THUAN");
// }
// void nghich()
// {
//    digitalWrite(triac1, 0);
//    digitalWrite(triac2, 1);
//    Serial.println("NGHICH");
// }

// void khoidong()
// {
//    while (count <= 400)
//       thuan();
//    while (count >= 0)
//       dung();
//    while (count >= -400)
//       nghich();
//    while (count <= 0)
//       dung();
//    while (count <= 800)
//       thuan();
//    while (count >= 0)
//       dung();
//    while (count >= -800)
//       nghich();
//    while (count <= 0)
//       dung();
//    while (count <= 1200)
//       thuan();
//    dung();
// }

void main()
{
   initLCD();

   // clear_interrupt(INT_EXT);
   // enable_interrupts(INT_EXT);
   // ext_int_edge(H_TO_L);
   // enable_interrupts(GLOBAL);

   // PORT_B_PULLUPS(0xff);

   // TRISB0 = TRISB1 = 1;                   //input
   // TRISB2 = TRISB3 = TRISB4 = TRISC2 = 0; //output
   TRISD1 = 0;

   while (TRUE)
   {
      ghima(0x80);
      hienthi("demo lcd");
      // hienthi(num[count / 10]);
      // hienthi(num[count % 10]);

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
