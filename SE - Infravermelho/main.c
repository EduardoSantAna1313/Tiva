#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>

#define CAPACIDADE 5

int main(void) {
	SYSCTL_RCGCGPIO_R = 0x09;
	SYSCTL_RCGCADC_R = 3;

	uint32_t leituraAtual1 = 0;
	uint32_t leituraAtual2 = 0;

	GPIO_PORTA_DIR_R = 60;
	GPIO_PORTA_DATA_R = 0x00;
	GPIO_PORTA_DEN_R = 60;

	GPIO_PORTD_AFSEL_R = 3;
	GPIO_PORTD_AMSEL_R = 3;

	ADC0_ACTSS_R = 0;
	ADC0_EMUX_R = 15;
	ADC0_SSMUX0_R = 7;
	ADC0_SSCTL0_R = 2;
	ADC0_ACTSS_R = 1;
	ADC0_PSSI_R = 1;

	ADC1_ACTSS_R = 0;
	ADC1_EMUX_R = 15;
	ADC1_SSMUX0_R = 6;
	ADC1_SSCTL0_R = 2;
	ADC1_ACTSS_R = 1;
	ADC1_PSSI_R = 1;

	uint32_t pessoas = 0;
	uint32_t vetorSentido[2] = {0, 0};
	uint32_t contVetorSentido = 0;

	while (1) {
		leituraAtual1 = ADC0_SSFIFO0_R;
		leituraAtual2 = ADC1_SSFIFO0_R;

		if (leituraAtual1 > 100 && leituraAtual2 < 100) {
			while(leituraAtual1 > 100){
				leituraAtual1 = ADC0_SSFIFO0_R;
			}
			vetorSentido[contVetorSentido] = 1;
			contVetorSentido++;
		}else if(leituraAtual2 > 100 && leituraAtual1 < 100){
			while(leituraAtual2 > 100){
				leituraAtual2 = ADC1_SSFIFO0_R;
			}
			vetorSentido[contVetorSentido] = 2;
			contVetorSentido++;
		}
		if(contVetorSentido == 2){
			if(vetorSentido[0] == 2 && vetorSentido[1] == 1 && pessoas > 0){
				// Saindo
				pessoas--;
			}else if(vetorSentido[0] == 1 && vetorSentido[1] == 2){
				//Entrando
				pessoas++;
			}
			contVetorSentido = 0;
		}

		if(pessoas == CAPACIDADE){
			GPIO_PORTA_DATA_R = 0x08; // Azul -> Lotado
		}else if(pessoas < CAPACIDADE){
			GPIO_PORTA_DATA_R = 0x10; // Verde
		}

	}

	return 0;
}
