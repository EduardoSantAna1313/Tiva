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

void UART_INT(void) {
	char ch = ler();
	escreverChar(ch);
	escreverChar('\n');
	if (ch == 'a') {
		piscarLED(0x02);
	}
}

int main(void) {
	inicializarUART();

	SYSCTL_RCGCGPIO_R |= 0x20;
	GPIO_PORTF_DIR_R = 0x02;
	GPIO_PORTF_DATA_R = 0x00;
	GPIO_PORTF_DEN_R = 0x02;

	escreverString("TESTE\n");
	while (1){
		__asm("WFI");
	}
	return 0;
}
