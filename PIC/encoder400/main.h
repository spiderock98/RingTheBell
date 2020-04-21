#include <16F877A.h>
#include <def_16f877a.h>

#device ADC = 16, WRITE_EEPROM = ASYNC

#FUSES NOWDT      //No Watch Dog Timer
#FUSES NOBROWNOUT //No brownout reset
#FUSES NOLVP      //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal = 20000000)

//================================================== PREPROSECSOR ==================================================

// 1 >> on
#define triac1Out PIN_D0
#define triac2Out PIN_D1
#define relayOut PIN_C0
// 0 >> pressed
#define btnINCREASE PIN_B4
#define btnDECREASE PIN_B1
// 1 >> led on
#define ledSAFETY PIN_D2
#define ledSTARTING PIN_D3
#define ledRINGING PIN_C5
#define ledBUTTON PIN_D4
// rst soft
#define PIN_RESET PIN_B5

#byte CMCON = 0x9C