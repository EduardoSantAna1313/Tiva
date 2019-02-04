#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>

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

	UART1_IBRD_R = 325;
	UART1_FBRD_R = 33;

	UART1_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);

	UART1_IFLS_R &= ~0x3F;
	UART1_IFLS_R += (UART_IFLS_TX1_8|UART_IFLS_RX1_8);

	UART1_IM_R |= (UART_IM_RXIM|UART_IM_TXIM|UART_IM_RTIM);
	UART1_CTL_R |= 0x01;

	GPIO_PORTB_LOCK_R 	|= 0x03;
	GPIO_PORTB_AFSEL_R 	|= 0x03;
	GPIO_PORTB_DEN_R 	|= 0x03;
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFFFF00)+0x00000011;
	NVIC_EN0_R = 1<<6;
}

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

void escrever(char c) {
	while ((UART1_FR_R & UART_FR_TXFF) != 0);
	UART1_DR_R = c;
}

char ler(void) {
	while ((UART1_FR_R & UART_FR_RXFE) != 0)
	{
	}
	return ((char) (UART1_DR_R & 0xFF));
}

void UART_INT(void) {
	char ch = ler();
	escrever(ch);
	escrever('\n');
	if (ch == 'r') {
		piscarLED(0x02);
	}else if(ch == 'g'){
		piscarLED(0x04);
	}else if(ch == 'b'){
		piscarLED(0x08);
	}
}

int main(void) {

	inicializarPLL();
	INIUART();

	SYSCTL_RCGCGPIO_R |= 0x20;
	GPIO_PORTF_DIR_R = 0x0E;
	GPIO_PORTF_DATA_R = 0x00;
	GPIO_PORTF_DEN_R = 0x0E;

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
		__asm("WFI");
	}

	return 0;
}
