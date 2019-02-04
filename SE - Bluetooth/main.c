#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>

#include "UART.h"

#define PORTA 0x01
#define PORTB 0x02
#define PORTC 0x04
#define PORTD 0x08
#define PORTE 0x10
#define PORTF 0x20

#define PIN0 0x01
#define PIN1 0x02
#define PIN2 0x04
#define PIN3 0x08
#define PIN4 0x10
#define PIN5 0x20
#define PIN6 0x30
#define PIN7 0x40

void piscarLED(uint32_t id) {
	uint32_t cont = 0;
	GPIO_PORTF_DATA_R |= id;
	for (cont = 0; cont < 100000; cont++);
	GPIO_PORTF_DATA_R &= 0x00;
	for (cont = 0; cont < 100000; cont++);
}

void INIUART() {

	SYSCTL_RCGCUART_R |= PORTB;
	SYSCTL_RCGCGPIO_R |= PORTB;

	UART1_CTL_R  &= ~UART_CTL_UARTEN;
	UART1_IBRD_R  = 325; // 27
	UART1_FBRD_R  = 34; // 8

	UART1_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
	UART1_CTL_R |= UART_CTL_UARTEN;

	GPIO_PORTB_LOCK_R   |= PIN0 | PIN1;
	GPIO_PORTB_AFSEL_R  |= PIN0 | PIN1;
	GPIO_PORTB_DEN_R 	|= PIN0 | PIN1;
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFFFF00)+0x00000011;
}

int main(void) {

	inicializarPLL();
	//INIUART();
	uint32_t PINS = PIN0 | PIN1;
	inicializarUART(PORTB, PINS);

	SYSCTL_RCGCGPIO_R |= 0x20;
	GPIO_PORTF_DIR_R = 0x0E;
	GPIO_PORTF_DATA_R = 0x00;
	GPIO_PORTF_DEN_R = 0x0E;

	char ch;

	uint32_t cont = 0;

	escrever('D');
	escrever('I');
	escrever('G');
	escrever('I');
	escrever('T');
	escrever('E');
	escrever(':');
	escrever('\n');

	while(1){
		ch = ler();

		if (ch == 'a') {
			piscarLED(0x02);
			escrever('V');
			escrever('E');
			escrever('R');
			escrever('M');
			escrever('E');
			escrever('L');
			escrever('H');
			escrever('O');
			escrever('\n');
		} else if (ch == 'b') {
			piscarLED(0x04);
			escrever('A');
			escrever('Z');
			escrever('U');
			escrever('L');
			escrever('\n');
		} else if (ch == 'c') {
			piscarLED(0x08);
			escrever('V');
			escrever('E');
			escrever('R');
			escrever('D');
			escrever('E');
			escrever('\n');
		} else if (ch == 'd') {
			piscarLED(0x06);
			escrever('R');
			escrever('O');
			escrever('X');
			escrever('O');
			escrever('\n');
		} else if (ch == 'e') {
			piscarLED(0x0A);
			escrever('A');
			escrever('M');
			escrever('A');
			escrever('R');
			escrever('E');
			escrever('L');
			escrever('O');
			escrever('\n');
		} else if (ch == 'f') {
			piscarLED(0x0E);
			escrever('B');
			escrever('R');
			escrever('A');
			escrever('N');
			escrever('C');
			escrever('O');
			escrever('\n');
		}

		for (cont = 0; cont < 10000; cont++);
	}

	return 0;
}
