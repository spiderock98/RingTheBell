#include <16F877A.h>
#include <def_16f877a.h>

#device ADC = 16, WRITE_EEPROM = ASYNC

#FUSES NOWDT      //No Watch Dog Timer
#FUSES NOBROWNOUT //No brownout reset
#FUSES NOLVP      //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal = 20000000)

//================================================== PREPROSECSOR ==================================================

// 1 >> on
#define triac1Out PIN_C6
#define triac2Out PIN_C7
// #define triac1Out PIN_D0
// #define triac2Out PIN_D1
#define relayOut PIN_C1

#byte CMCON = 0x9C