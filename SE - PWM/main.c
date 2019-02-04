#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>
#define FREQ 10

int main(void) {
	SYSCTL_RCGCGPIO_R = 0x02; // PORTB

	GPIO_PORTB_AFSEL_R = 1 << 6;	// Função alternativa
	GPIO_PORTB_PCTL_R  = 1 << 26;	// Controle
	GPIO_PORTB_DEN_R   = 1 << 6;	// Digital

	PWM0_0_GENA_R |= 0x000000C8;  // Comparador A -> Define configurações pag. 1282
	PWM0_0_LOAD_R = FREQ;	// Frequencia de PWM - > Clock sistema
	PWM0_0_CMPA_R |= ((FREQ * 50 )/100);	//
	PWM0_0_CTL_R |=  0x01;	// começa a contar
	//PWM_ENABLE_PWM0EN |= 0x01;

	while(1)
		__asm("WFI");

	return 0;
}
