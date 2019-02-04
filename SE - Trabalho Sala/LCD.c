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

void delay(uint32_t us){
	uint32_t cont = 0;
	for(cont = 0; cont < us*40; cont++);
}

void outCmd(unsigned char command) {
	GPIO_PORTB_DATA_R = command;
	GPIO_PORTA_DATA_R = 0x00; // E=0, R/W=0, RS=0
	delay(6);
	GPIO_PORTA_DATA_R = 0x80; // ENABLE = 1, R/W = 0, RS = 0
	delay(6);
	GPIO_PORTA_DATA_R = 0x00; // E=0, R/W=0, RS=0
	delay(40);
}

void inicializarLCD(void) {
	SYSCTL_RCGCGPIO_R |= 0x03;
	GPIO_PORTB_DIR_R = 0xFF;
	GPIO_PORTB_DATA_R = 0x00;
	GPIO_PORTB_DEN_R = 0xFF;

	GPIO_PORTA_DIR_R = 0xC0;
	GPIO_PORTA_DATA_R = 0x00;
	GPIO_PORTA_DEN_R = 0xC0;

	GPIO_PORTA_DATA_R = 0;
	delay(1500);

	outCmd(0x30);         // command 0x30 = Wake up
	delay(500);
	outCmd(0x30);         // command 0x30 = Wake up #2
	delay(16);
	outCmd(0x30);         // command 0x30 = Wake up #3
	delay(16);
	outCmd(0x38);         // Function set: 8-bit/2-line
	outCmd(0x10);         // Set setCursor
	outCmd(0x0C);         // Display ON; setCursor ON
	outCmd(0x06);         // Entry mode set
}

void printChar(unsigned char c) {
	GPIO_PORTB_DATA_R = c;
	GPIO_PORTA_DATA_R = 0x40;	// E=0, R/W=0, RS=1
	delay(6);
	GPIO_PORTA_DATA_R = 0xC0;	// E=1, R/W=0, RS=1
	delay(6);
	GPIO_PORTA_DATA_R = 0x40;	// E=0, R/W=0, RS=1
	delay(40);
}

void clear(void) {
	outCmd(0x01);
	delay(1600);
}

void printString(char *ponteiro) {
	while (*ponteiro) {
		printChar(*ponteiro);
		ponteiro++;
	}
}

void printInt(uint32_t num) {
	if (num >= 10) {
		printInt(num / 10);
		num = num % 10;
	}
	printChar(num + '0');
}

void setCursor(uint8_t lin, uint8_t col){
	if(lin == 1){
		outCmd(0x02);
		uint32_t cs = 0;
		for(cs = 0; cs < col; cs ++){
			outCmd(0x14);
		}
		//outCmd(col);
	}else if(lin == 2){
		outCmd(0xC0 + col);
	}
	delay(1600);
}
