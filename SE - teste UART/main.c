#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>
#include "UART.h"

void piscarLED(uint32_t id) {
	uint32_t cont = 0;
	GPIO_PORTF_DATA_R |= id;
	for (cont = 0; cont < 100000; cont++);
	GPIO_PORTF_DATA_R &= 0x00;
	for (cont = 0; cont < 100000; cont++);
}

int main(void) {
	uint32_t PORT = 0x01;
	uint32_t PINS = 0x03;

	inicializarPLL();
	inicializarUART(PORT, PINS);

	SYSCTL_RCGCGPIO_R |= 0x20;
	GPIO_PORTF_DIR_R = 0x0E;
	GPIO_PORTF_DATA_R = 0x00;
	GPIO_PORTF_DEN_R = 0x0E;

	char ch;

	while (1) {
		uint32_t cont = 0;
		ch = ler();
		if (ch == 'a') {
			piscarLED(0x02);
			escrever("RED\n");
		} else if (ch == 'b') {
			piscarLED(0x04);
			escrever("BLUE\n");
		} else if (ch == 'c') {
			piscarLED(0x08);
			escrever("GREEN\n");
		} else if (ch == 'd') {
			piscarLED(0x06);
			escrever("COR\n");
		} else if (ch == 'e') {
			piscarLED(0x0A);
			escrever("COR\n");
		} else if (ch == 'f') {
			piscarLED(0x0E);
			escrever("COR\n");
		}
		for (cont = 0; cont < 10000; cont++);
	}
	return 0;
}
