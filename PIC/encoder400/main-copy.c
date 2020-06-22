#include <main.h>
#include "floor.c"

//================================================== VARIABLES ==================================================

// set angle (10 levels). more angle, weaker motor
int32 angleStarter = 9;
int32 angleRingTheBell = 9;

int32 valTimer0SetStarter = (int32)FLOOR((13.1072 - angleStarter) / 0.0512) - 1;
int32 valTimer0SetRingTheBell = (int32)FLOOR((13.1072 - angleRingTheBell) / 0.0512) - 1;

volatile signed int16 count = 0;
signed int8 countOverload = 10;
int1 flagForward = true, flagStarter = true, flagSTOP = true, flagOverload = false;

//================================================== ISR Func() ==================================================

#INT_EXT
void ext_isr()
{
   // cross zero point detector 50Hz >> 1/50/2 sec per half pulse after diode brighe

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

#INT_TIMER2
void timer2_isr()
{
   // if (count >= ProtectRotate) // (+)ProtectRotate rotate
   // {
   //    // output_low(relayOut); // safety switch
   //    // reset_cpu();
   //    flagOverload = true;
   // }
   if (count <= -1 * ProtectRotate) // (-)ProtectRotate rotate
   {
      // output_low(relayOut); // safety switch
      // reset_cpu();
      flagOverload = true;
   }
}

#INT_TIMER0
void timer0_isr()
{
   if (flagForward && !flagSTOP)
      output_high(triac1Out);
   else if (!flagForward && !flagSTOP)
      output_high(triac2Out);

   disable_interrupts(INT_TIMER0);
}

#INT_CCP1
void ccp1_isr()
{
   if (RC1)
   {
      ++count;
   }

   else
   {
      --count;
   }
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
   int8 checkEncoderCount = 1000;
   while (count <= 50)
   {
      FORWARD();
      if (count <= -50)
         reset_cpu(); // return

      if (count == 0) // check encoder respone if stuck in 0 point
      {
         if (!(--checkEncoderCount))
            reset_cpu();
         delay_ms(1); // in <checkEncoderCount> ms
      }
   }
   STOP();
   while (count >= 12)
      STOP();
   while (count >= -50)
      REVERSE();
   STOP();
   while (count <= -12)
      STOP();

   // stage 1
   for (signed int16 i = 50; i < 500; i += 50)
   {
      while (count <= i)
         FORWARD();
      STOP();
      while (count >= (i * 3 / 4))
         STOP();
      while (count >= -i)
         REVERSE();
      STOP();
      while (count <= -(i * 3 / 4))
         STOP();
   }

   // stage 2
   for (signed int16 ii = 500; ii < 800; ii += 100)
   {
      while (count <= (ii * 3 / 4))
         FORWARD();
      STOP();
      while (count >= (ii / 2))
         STOP();
      while (count >= -(ii * 3 / 4))
         REVERSE();
      STOP();
      while (count <= -(ii / 2))
         STOP();
   }

   flagStarter = false;
}

void RingTheBell()
{
   if (flagOverload)
   {
      if (countOverload--)
      {
         while (count <= 350)
            STOP();
         STOP();

         while (count >= 250)
            STOP();

         while (count >= -350)
            STOP();

         while (count <= -250)
            STOP();
      }
      else
      {
         countOverload = 10;
         flagOverload = false;
      }
   }
   else
   {
      while (count <= 350)
         FORWARD();
      STOP();

      while (count >= 250)
         STOP();

      while (count >= -350)
         REVERSE();
      STOP();

      while (count <= -250)
         STOP();
   }
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
      if (((count - lastCount) > 40) || ((count - lastCount) < -40))
      {
         i = sec; // reset
         lastCount = count;
         delay_ms(30);
      }
   }

   count = 0; // update 0 point
   output_high(relayOut);
   delay_ms(3000); // ngăn hồ quang nếu cùng lúc đóng triac lập tức
   flagSTOP = false;
}

//================================================== MAIN ==================================================

void main()
{
   TRISB0 = TRISC1 = TRISC2 = 1;          //input
   TRISC0 = TRISD2 = TRISD3 = TRISC5 = 0; //output  v6, v5
   //TRISC0 = TRISD2 = TRISD3 = TRISC4 = 0; //output    v66

   // triac
   TRISD0 = TRISD1 = 0; //v6
   //TRISC6 = TRISC7 = 0;     //v5

   output_low(triac1Out);
   output_low(triac2Out);
   output_low(ledSAFETY);   // CLEAR reset pin
   output_low(ledSTARTING); // CLEAR reset pin
   output_low(ledRINGING);  // CLEAR reset pin

   clear_interrupt(INT_EXT);
   ext_int_edge(H_TO_L);

   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256); // 51.2us

   setup_ccp1(CCP_CAPTURE_FE);
   setup_timer_1(T1_INTERNAL);
   enable_interrupts(INT_CCP1);

   setup_timer_2(T2_DIV_BY_16, 255, 16); // Overflow every 13.056us
   set_timer2(0);

   enable_interrupts(GLOBAL);

   output_high(ledSAFETY);
   checkSafetyFirst(1300000); // ~~ 6 seconds
   output_low(ledSAFETY);

   enable_interrupts(INT_TIMER0);
   enable_interrupts(INT_EXT);
   enable_interrupts(INT_TIMER2);

   output_high(ledSTARTING);
   starter();
   output_low(ledSTARTING);

   output_high(ledRINGING);
   while (TRUE)
   {
      RingTheBell();
   }
}
