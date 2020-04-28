#include <main.h>
#include "floor.c"

//================================================== VARIABLES ==================================================

// set angle (10 levels). more angle, weaker motor
int32 angleStarter = 1;
int32 angleRingTheBell = 6;

int32 valTimer0SetStarter = (int32)FLOOR((13.1072 - angleStarter) / 0.0512) - 1;
int32 valTimer0SetRingTheBell = (int32)FLOOR((13.1072 - angleRingTheBell) / 0.0512) - 1;

volatile signed int16 count = 0;
int1 flagForward = true, flagStarter = true, flagSTOP = true;

signed int16 ProtectRotate = 2000;

//================================================== ISR Func() ==================================================

#INT_EXT
void ext_isr()
{
   // cross zero point detector 50Hz >> 1/50/2 sec per half pulse after diode brighet

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

#INT_TIMER2
void timer2_isr()
{
   if (count >= ProtectRotate) // (+)ProtectRotate rotate
   {
      output_low(relayOut); // safety switch
      reset_cpu();
   }
   if (count <= -1 * ProtectRotate) // (-)ProtectRotate rotate
   {
      output_low(relayOut); // safety switch
      reset_cpu();
   }
}

#INT_CCP1
void ccp1_isr()
{
   if (RC0)
      ++count;
   else
      --count;
}

//================================================== Func() ==================================================

void FORWARD()
{
   output_low(triac2Out);

   flagForward = true;
   flagSTOP = false;
}
void REVERSE()
{
   output_low(triac1Out);
   flagForward = flagSTOP = false;
}
void STOP()
{
   flagSTOP = true;
   output_low(triac1Out);
   output_low(triac2Out);
}

void starter()
{
   // half rotate
   while (count <= 200)
   {
      if (count <= -200)
         reset_cpu();
      FORWARD();
   }

   STOP();
   while (count >= 0)
      STOP();
   while (count >= -200)
      REVERSE();
   STOP();
   while (count <= 0)
      STOP();

   // 1 rotate
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

   // 2 rotates
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

   // 3 rotates 1 side
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

// pic 20mhz 5.10^6 = 1sec
// motor 2000rpm
// quay co tai: 0.5rps -> 200xung/s -> 1/200 s/xung
void checkSafetyFirst(int32 sec)
{
   flagSTOP = true;
   output_low(relayOut); // safety switch

   signed int16 lastCount = count;
   for (int32 i = sec; --i;) // waitting steady
   {
      if (count != lastCount)
      {
         i = sec; // reset
         lastCount = count;
         delay_ms(30); // pray for couting up
      }
   }

   count = 0; // update 0 point
   output_high(relayOut);
   delay_ms(3000); // ngăn hồ quang nếu cùng lúc đóng triac lập tức
   enable_interrupts(INT_TIMER2);
   flagSTOP = false;
}

//================================================== MAIN ==================================================

void main()
{
   // encoder dir
   TRISC0 = TRISC2 = 1;

   // cross zero detector dir
   TRISB0 = 1;

   // Triac dir
   // TRISD0 = TRISD1 = 0;
   TRISC6 = TRISC7 = 0;

   // relay dir
   TRISC1 = 0;

   clear_interrupt(INT_EXT);
   enable_interrupts(INT_EXT);
   ext_int_edge(H_TO_L);

   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256); // 51.2us
   enable_interrupts(INT_TIMER0);

   setup_ccp1(CCP_CAPTURE_FE);
   setup_timer_1(T1_INTERNAL);
   enable_interrupts(INT_CCP1);

   setup_timer_2(T2_DIV_BY_16, 255, 16); // Overflow every 13.056us
   set_timer2(0);

   enable_interrupts(GLOBAL);

   checkSafetyFirst(1500000); // ~~ 6 seconds

   starter();

   while (TRUE)
   {
      RingTheBell();
   }
}
