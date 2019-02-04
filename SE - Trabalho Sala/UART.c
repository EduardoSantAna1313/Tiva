/*
 * PORT_A
 * PIN0 e PIN1
 */

#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>
#include "LCD.h"

void inicializarUART() {

	SYSCTL_RCGCUART_R |= 0x01;
	//SYSCTL_RCGCGPIO_R |= 0x01;
	delay(160);
	UART0_IBRD_R = 104;
	UART0_FBRD_R = 11;

	UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);

	UART0_IFLS_R &= ~0x3F;
	UART0_IFLS_R += (UART_IFLS_TX1_8|UART_IFLS_RX1_8);

	UART0_IM_R |= (UART_IM_RXIM|UART_IM_TXIM|UART_IM_RTIM);
	UART0_CTL_R |= 0x01;

	delay(160);

	GPIO_PORTA_LOCK_R 	|= 0x03;
	GPIO_PORTA_AFSEL_R 	|= 0x03;
	GPIO_PORTA_DEN_R 	|= 0x03;
	GPIO_PORTA_PCTL_R   |= (GPIO_PORTA_PCTL_R & 0xFFFFFF00)+0x00000011;
	delay(160);
	NVIC_EN0_R |= 1<<5;
	delay(160);
}

void escrever(char c) {
	while ((UART0_FR_R & UART_FR_TXFF) != 0);
	UART0_DR_R = c;
}

void uartEscrever(char *ponteiro){
	while(*ponteiro){
		escrever(*ponteiro);
		ponteiro++;
	}
}

char uartLer(void) {
	while ((UART0_FR_R & UART_FR_RXFE) != 0);
	return ((char) (UART0_DR_R & 0xFF));
}




