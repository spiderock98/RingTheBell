#include <main.h>
#include "floor.c"

//================================================== VARIABLES ==================================================

// set angle (10 levels). more angle, weaker motor
volatile int32 angleStarter = 1;
volatile int32 angleRingTheBell = 6;
// volatile int32 angleStarter = read_eeprom(0x00);
// volatile int32 angleRingTheBell = read_eeprom(0x01);

volatile int32 valTimer0SetStarter = (int32)FLOOR((13.1072 - angleStarter) / 0.0512) - 1;
volatile int32 valTimer0SetRingTheBell = (int32)FLOOR((13.1072 - angleRingTheBell) / 0.0512) - 1;

volatile signed int16 count = 0;
volatile int16 iTimer2OverFlow;
int1 flagForward = true, flagStarter = true, flagSTOP = true;

//================================================== ISR Func() ==================================================

// #INT_COMP
// void isrComparator()
// { // A mismatch condition will continue to set flag bit CMIF. Reading CMCON will end the mismatch condition and allow flag bit CMIF to be cleared
//    char charas = CMCON;
//    output_low(relayOut);   // safety switch
//    output_high(PIN_RESET); // reset mcu
// }

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
   // --iTimer2OverFlow;
   // if (!(--iTimer2OverFlow))
   // if (!iTimer2OverFlow)
   {
      // reset neu quay qua 4 vong
      if (flagForward)
      {
         if (count >= 1600) // (+)4 rotate
         {
            output_low(relayOut); // safety switch
            reset_cpu();
         }
      }
      else
      {
         if (count <= -1600) // (-)4 rotate
         {
            output_low(relayOut); // safety switch
            reset_cpu();
         }
      }

      if (!input(btnDECREASE))
      {
         output_high(ledBUTTON);
         if (flagStarter)
         {
            if (angleStarter < 9)
            {
               ++angleStarter;
               // write_eeprom(0x00, ++angleStarter);
               valTimer0SetStarter = (int32)FLOOR((13.1072 - angleStarter) / 0.0512) - 1;
            }
         }
         else
         {
            if (angleRingTheBell < 9)
            {
               ++angleRingTheBell;
               // write_eeprom(0x01, ++angleRingTheBell);
               valTimer0SetRingTheBell = (int32)FLOOR((13.1072 - angleRingTheBell) / 0.0512) - 1;
            }
         }
         output_low(ledBUTTON);
      }
      else if (!input(btnINCREASE))
      {
         output_high(ledBUTTON);
         if (flagStarter)
         {
            if (angleStarter > 1)
            {
               --angleStarter;
               // write_eeprom(0x00, --angleStarter);
               valTimer0SetStarter = (int32)FLOOR((13.1072 - angleStarter) / 0.0512) - 1;
            }
         }
         else
         {
            if (angleRingTheBell > 1)
            {
               --angleRingTheBell;
               // write_eeprom(0x01, --angleRingTheBell);
               valTimer0SetRingTheBell = (int32)FLOOR((13.1072 - angleRingTheBell) / 0.0512) - 1;
            }
         }
         output_low(ledBUTTON);
      }

      // iTimer2OverFlow = 50; // 100ms every command
      // iTimer2OverFlow = 7660; // 100ms every command
   }
}

#INT_CCP1
void ccp1_isr()
{
   if (RC1)
      ++count;
   else
      --count;
   // clear_interrupt(INT_CCP1);
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
      FORWARD();
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
   flagSTOP = false;
}

//================================================== MAIN ==================================================

void main()
{
   TRISB0 = TRISC1 = TRISC2 = TRISB1 = TRISB2 = TRISB3 = TRISB4 = 1;          //input
   TRISD0 = TRISD1 = TRISC0 = TRISD2 = TRISD3 = TRISC5 = TRISD4 = TRISB5 = 0; //output

   output_low(ledSAFETY);   // CLEAR reset pin
   output_low(ledSTARTING); // CLEAR reset pin
   output_low(ledRINGING);  // CLEAR reset pin

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
   // iTimer2OverFlow = 50; // 100ms every command
   // iTimer2OverFlow = 7660; // 100ms every command
   enable_interrupts(INT_TIMER2);

   // setup_comparator(A0_VR_A1_VR);
   // setup_vref(VREF_HIGH | 5);
   // enable_interrupts(INT_COMP);

   enable_interrupts(GLOBAL);

   output_high(ledSAFETY);
   // checkSafetyFirst(15); // ~~ 6 seconds
   checkSafetyFirst(1500000); // ~~ 6 seconds
   output_low(ledSAFETY);

   output_high(ledSTARTING);
   starter();
   output_low(ledSTARTING);

   output_high(ledRINGING);
   while (TRUE)
   {
      RingTheBell();
   }
}
