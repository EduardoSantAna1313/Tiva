#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>

void inicializarPLL(void) {
	SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;
	SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;
	SYSCTL_RCC_R &= ~SYSCTL_RCC_XTAL_M;
	SYSCTL_RCC_R += SYSCTL_RCC_XTAL_16MHZ;
	SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;
	SYSCTL_RCC2_R += SYSCTL_RCC2_OSCSRC2_MO;
	SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;
	SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;
	SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~0x1FC00000) + (7 << 22);

	while ((SYSCTL_RIS_R & SYSCTL_RIS_PLLLRIS) == 0)
	{
	}
	SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;
}

void inicializarUART(uint32_t PORT, uint32_t PINS) {

	SYSCTL_RCGCUART_R |= PORT;
	SYSCTL_RCGCGPIO_R |= PORT;

	UART0_CTL_R  &= ~UART_CTL_UARTEN;
	UART0_IBRD_R  = 27;
	UART0_FBRD_R  = 8;

	UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
	UART0_CTL_R |= UART_CTL_UARTEN;

	if(PORT == 0x01){
		GPIO_PORTA_LOCK_R   |= PINS;
		GPIO_PORTA_AFSEL_R  |= PINS;
		GPIO_PORTA_DEN_R    |= PINS;
		GPIO_PORTA_PCTL_R   &= (GPIO_PORTA_PCTL_R & 0xFFFFFF00)+0x00000011;
	}else if(PORT == 0x02){
		GPIO_PORTB_LOCK_R   |= PINS;
		GPIO_PORTB_AFSEL_R  |= PINS;
		GPIO_PORTB_DEN_R 	|= PINS;
		GPIO_PORTB_PCTL_R 	&= PINS;
	}else if(PORT == 0x04){
		GPIO_PORTC_LOCK_R   |= PINS;
		GPIO_PORTC_AFSEL_R  |= PINS;
		GPIO_PORTC_DEN_R 	|= PINS;
		GPIO_PORTC_PCTL_R 	&= PINS;
	}else if(PORT == 0x08){
		GPIO_PORTD_LOCK_R   |= PINS;
		GPIO_PORTD_AFSEL_R  |= PINS;
		GPIO_PORTD_DEN_R 	|= PINS;
		GPIO_PORTD_PCTL_R 	&= PINS;
	}else if(PORT == 0x10){
		GPIO_PORTE_LOCK_R   |= PINS;
		GPIO_PORTE_AFSEL_R  |= PINS;
		GPIO_PORTE_DEN_R 	|= PINS;
		GPIO_PORTE_PCTL_R 	&= PINS;
	}else if(PORT == 0x20){
		GPIO_PORTF_LOCK_R   |= PINS;
		GPIO_PORTF_AFSEL_R  |= PINS;
		GPIO_PORTF_DEN_R 	|= PINS;
		GPIO_PORTF_PCTL_R 	&= PINS;
	}

}

void escrever(char *ponteiro) {
	while ((UART0_FR_R & UART_FR_TXFF) != 0);
	while (*ponteiro) {
		UART0_DR_R = (*ponteiro);
		ponteiro++;
	}
}

char ler(void) {
	while ((UART0_FR_R & UART_FR_RXFE) != 0)
	{
	}
	return ((char) (UART0_DR_R & 0xFF));
}
