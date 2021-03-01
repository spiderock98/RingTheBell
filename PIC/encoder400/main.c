#include <main.h>
#include "floor.c"

#define initFeedbackCount 10
//================================================== VARIABLES ==================================================

// set angle (10 levels). more angle, weaker motor
int32 angleStarter = 9;
int32 angleRingTheBell = 9;
// volatile int32 angleStarter = read_eeprom(0x00);
// volatile int32 angleRingTheBell = read_eeprom(0x01);

// int32 valTimer0SetStarter = (int32)FLOOR((13.1072 - angleStarter) / 0.0512) - 1;
// int32 valTimer0SetRingTheBell = (int32)FLOOR((13.1072 - angleRingTheBell) / 0.0512) - 1;

int32 valTimer0SetStarter = 0;     // init in DipSwitchState()
int32 valTimer0SetRingTheBell = 0; // init in DipSwitchState()

volatile signed int16 count = 0;
// volatile int16 iTimer2OverFlow;
int1 flagForward = true, flagStarter = true, flagSTOP = true;

volatile int1 flagFbForward = true;
volatile int8 countFbForward = initFeedbackCount, countFbReverse = initFeedbackCount; // reset after 10 pulse suddendly change direction

signed int16 ProtectRotate = 0; // init in DipSwitchState()

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
   // cross zero point detector 50Hz >> 1/50/2 sec per half pulse after diode bridge

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
      // if (flagForward)
      // {
      if (count >= ProtectRotate) // (+)ProtectRotate rotate
      {
         output_low(relayOut); // safety switch
         reset_cpu();
      }
      // }
      //else
      //{
      if (count <= -1 * ProtectRotate) // (-)ProtectRotate rotate
      {
         output_low(relayOut); // safety switch
         reset_cpu();
      }
      //}
   }
}

#INT_CCP1
void ccp1_isr()
{
   if (RC1)
   {
      ++count;
      flagFbForward = true;
   }

   else
   {
      --count;
      flagFbForward = false;
   }
}

//================================================== Func() ==================================================

void FORWARD()
{
   output_low(triac2Out);

   flagForward = true;
   flagSTOP = false;
   countFbForward = initFeedbackCount; // ready for next change dir

   if (!flagFbForward) // suddendly reverse for n pulses
   {
      if (!(--countFbReverse))
         reset_cpu();

      return;
   }
   countFbReverse = initFeedbackCount; // reset if not in 10 pulses series
}
void REVERSE()
{
   output_low(triac1Out);

   flagForward = flagSTOP = false;
   countFbReverse = initFeedbackCount; // ready for next change dir

   if (flagFbForward) // suddendly forward for 10 pulses
   {
      if (!(--countFbForward))
         reset_cpu();
      return;
   }
   countFbForward = initFeedbackCount; // reset if not in 10 pulses series
}
void STOP()
{
   flagSTOP = true;
   output_low(triac1Out);
   output_low(triac2Out);
}

void starter()
{
   while (count <= 100)
   {
      if (count <= -100)
         reset_cpu(); // return
      FORWARD();
   }
   STOP();
   while (count >= 50)
      STOP();
   while (count >= -100)
      REVERSE();
   STOP();
   while (count <= -50)
      STOP();

   for (signed int16 i = 100; i < 1100; i += 100)
   {
      while (count <= i)
         FORWARD();
      STOP();
      while (count >= i / 2)
         STOP();
      while (count >= -i)
         REVERSE();
      STOP();
      while (count <= -i / 2)
         STOP();
   }

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
      if (((count - lastCount) > 10) || ((count - lastCount) < -10))
      {
         i = sec; // reset
         lastCount = count;
         delay_ms(30); // pray for counting change
      }
   }

   count = 0; // update 0 point
   output_high(relayOut);
   delay_ms(3000); // ngăn hồ quang nếu cùng lúc đóng triac lập tức
   enable_interrupts(INT_TIMER2);
   flagSTOP = false;
}

void initDipSwitchState()
{
   TRISB5 = TRISB4 = TRISB3 = TRISB2 = TRISB1 = TRISD6 = TRISD5 = 1;

   // Dip swith Start
   switch (PORTB & 0b00110000)
   {
   case 0:
      angleStarter = 0;
      // output_high(ledRINGING); // debug
      // delay_ms(100);
      // output_low(ledRINGING);
      // delay_ms(100);
      // output_high(ledRINGING);
      // delay_ms(100);
      // output_low(ledRINGING);
      break;
   case 0b00010000:
      angleStarter = 1;
      break;
   case 0b00100000:
      angleStarter = 2;
      break;
   case 0b00110000:
      angleStarter = 3;
      break;
   }

   // Dip swith Ringing
   switch (PORTB & 0b00001110)
   {
   case 0b0000:
      angleRingTheBell = 2;
      break;
   case 0b0010:
      angleRingTheBell = 3;
      break;
   case 0b0100:
      angleRingTheBell = 4;
      break;
   case 0b0110:
      angleRingTheBell = 5;
      break;
   case 0b1000:
      angleRingTheBell = 6;
      break;
   case 0b1010:
      angleRingTheBell = 7;
      break;
   case 0b1100:
      angleRingTheBell = 8;
      break;
   case 0b1110:
      angleRingTheBell = 9;
      break;
   }

   // Dip swith Protector Rotate
   switch (PORTD & 0b01100000)
   {
   case 0b0000000:
      ProtectRotate = 2000;
      break;
   case 0b0100000:
      ProtectRotate = 2400;
      break;
   case 0b1000000:
      ProtectRotate = 2800;
      break;
   case 0b1100000:
      ProtectRotate = 3200;
      break;
   }
}

//================================================== MAIN ==================================================

void main()
{
   TRISB0 = TRISC1 = TRISC2 = 1;          //input
   TRISC0 = TRISD2 = TRISD3 = TRISC5 = 0; //output

   // triac
   // TRISD0 = TRISD1 = 0;
   TRISC6 = TRISC7 = 0;

   initDipSwitchState();
   valTimer0SetStarter = (int32)FLOOR((13.1072 - angleStarter) / 0.0512) - 1;
   valTimer0SetRingTheBell = (int32)FLOOR((13.1072 - angleRingTheBell) / 0.0512) - 1;

   output_low(triac1Out);
   output_low(triac2Out);

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

   // setup_comparator(A0_VR_A1_VR);
   // setup_vref(VREF_HIGH | 5);
   // enable_interrupts(INT_COMP);

   enable_interrupts(GLOBAL);

   output_high(ledSAFETY);
   checkSafetyFirst(1300000); // ~~ 6 seconds
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
