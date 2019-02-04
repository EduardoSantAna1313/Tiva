#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>

int main(void) {
	SYSCTL_RCGCGPIO_R = 1 << 3;
	SYSCTL_RCGCADC_R = 1;

	GPIO_PORTD_AFSEL_R = 1;
	GPIO_PORTD_AMSEL_R = 1;

	ADC0_ACTSS_R = 0;
	ADC0_EMUX_R = 15;
	ADC0_SSMUX0_R = 7;
	ADC0_SSCTL0_R = 2;
	ADC0_ACTSS_R = 1;
	ADC0_PSSI_R = 1;

	uint32_t leituraAtual;

	while(1){
		leituraAtual = ADC0_SSFIFO0_R;
	}
	
	return 0;
}
