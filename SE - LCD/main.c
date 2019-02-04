/*
 VSS -> GND
 VDD -> 3.3V
 V0  -> GND
 RS  -> PA6
 R/W -> GND
 E   -> PA7
 D0  -> PB0
 ...
 D7  -> PB7
 */

#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>
#include "LCD.h"

int main(void) {
	inicializarLCD();
	clear();
	setCursor(1,0);
	printString("0123456789ABCDEF");
	setCursor(2,0);
	printString("Linha 2");
	while(1);
}
