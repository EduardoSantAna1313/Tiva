#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>

void inicializarUART() {
	SYSCTL_RCGCUART_R |= 0x01;
	SYSCTL_RCGCGPIO_R |= 0x01;

	UART0_IBRD_R = 104;
	UART0_FBRD_R = 11;

	UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);

	UART0_IFLS_R &= ~0x3F;
	UART0_IFLS_R += (UART_IFLS_TX1_8|UART_IFLS_RX1_8);

	UART0_IM_R |= (UART_IM_RXIM|UART_IM_TXIM|UART_IM_RTIM);
	UART0_CTL_R |= 0x01;

	GPIO_PORTA_LOCK_R   |= 0x03;
	GPIO_PORTA_AFSEL_R  |= 0x03;
	GPIO_PORTA_DEN_R    |= 0x03;
	GPIO_PORTA_PCTL_R   &= (GPIO_PORTA_PCTL_R & 0xFFFFFF00)+0x00000011;
	NVIC_EN0_R |= 1<<5;
}

void escreverChar(char ch){
	while ((UART0_FR_R & UART_FR_TXFF) != 0);
	UART0_DR_R = ch;
}

void escreverString(char *string) {
	while (*string) {
		escreverChar(*string);
		string++;
	}
}

char ler(void) {
	UART0_ICR_R = UART_ICR_RXIC;
	UART0_ICR_R = UART_ICR_TXIC;
	while ((UART0_FR_R & UART_FR_RXFE) != 0);
	return ((char) (UART0_DR_R & 0xFF));
}
