#include <main.h>
#include "../floor.c"

#define triac1 RC6

// set angle (10 levels). more angle, weaker motor
float32 angleStarter = 1;
int32 valTimer0SetStarter = (int32)FLOOR((13.1072 - angleStarter) / 0.0512) - 1;

int8 countTime = 0;
int8 anglePercent = 0;

#INT_EXT
void EXT_isr(void)
{
   // cross zero point detector 50Hz >> 1/50/2 sec per half pulse after diode brighet
   triac1 = 0;

   set_timer0(valTimer0SetStarter);

   clear_interrupt(INT_TIMER0);
   enable_interrupts(INT_TIMER0);
   clear_interrupt(INT_EXT); // waiting for next EXT
}

#INT_TIMER0
void TIMER0_isr(void)
{
   triac1 = 1;

   disable_interrupts(INT_TIMER0);
}

#INT_TIMER1
void TIMER1_isr(void)
{
   ++countTime;
   if ((10 - angleStarter) && (countTime == 50))
   {
      countTime = 0;
      angleStarter += 1;
      valTimer0SetStarter = (int32)FLOOR((13.1072 - angleStarter) / 0.0512) - 1;
   }

   clear_interrupt(INT_TIMER1);
   set_timer1(3036); // every 100ms
}

void main()
{
   TRISC6 = 0;
   TRISB0 = 1;
   //TRISA = 0;

   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256); //51.2us
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);    // 1.6us
   set_timer1(3036);                            //100ms

   enable_interrupts(INT_EXT);
   // enable_interrupts(INT_TIMER0);
   enable_interrupts(INT_TIMER1);
   enable_interrupts(GLOBAL);

   while (TRUE)
   {
      //TODO: User Code
   }
}
