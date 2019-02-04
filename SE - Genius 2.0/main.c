//PD0 - azul
//PD1 - vermelho
//PD2 - amarelo
//PD3 - perdeu

//PE1 - botao azul
//PA4 - botao vermelho
//PB2 - botao amarelo
#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>

#define tempoHigh 250000
#define tempoLow 1000

void ligaAzul() {
	volatile uint32_t ui32Loop;
	GPIO_PORTD_DATA_R |= 0x01;
	for (ui32Loop = 0; ui32Loop < tempoHigh; ui32Loop++) {
	}
	GPIO_PORTD_DATA_R &= 0x00;
	for (ui32Loop = 0; ui32Loop < tempoHigh; ui32Loop++) {
	}
}

void ligaVermelho() {
	volatile uint32_t ui32Loop;
	GPIO_PORTD_DATA_R |= 0x02;
	for (ui32Loop = 0; ui32Loop < tempoHigh; ui32Loop++) {
	}
	GPIO_PORTD_DATA_R &= 0x00;
	for (ui32Loop = 0; ui32Loop < tempoHigh; ui32Loop++) {
	}
}

void ligaAmarelo() {
	volatile uint32_t ui32Loop;
	GPIO_PORTD_DATA_R |= 0x04;
	for (ui32Loop = 0; ui32Loop < tempoHigh; ui32Loop++) {
	}
	GPIO_PORTD_DATA_R &= 0x00;
	for (ui32Loop = 0; ui32Loop < tempoHigh; ui32Loop++) {
	}
}

void venceu() {
	volatile uint32_t ui32Loop;
	int i;
	for (i = 0; i < 3; i++) {
		GPIO_PORTD_DATA_R |= 0x0F;
		for (ui32Loop = 0; ui32Loop < tempoHigh; ui32Loop++);
		GPIO_PORTD_DATA_R &= 0x00;
		for (ui32Loop = 0; ui32Loop < tempoHigh; ui32Loop++);
	}

}

void perdeu() {
	volatile uint32_t ui32Loop;
	int i;
	for (i = 0; i < 3; i++) {
		GPIO_PORTD_DATA_R |= 0x08;
		for (ui32Loop = 0; ui32Loop < tempoHigh; ui32Loop++);
		GPIO_PORTD_DATA_R &= 0x00;
		for (ui32Loop = 0; ui32Loop < tempoHigh; ui32Loop++);
	}

}

uint32_t esperaBotao(){
	uint32_t saida = 0;

	uint32_t statusAzul;
	uint32_t statusVermelho;
	uint32_t statusAmarelo;

	uint32_t clicou = 0;

	while(clicou == 0){
		statusAzul = GPIO_PORTE_DATA_R;     // E1
		statusVermelho = GPIO_PORTA_DATA_R; // C4
		statusAmarelo = GPIO_PORTB_DATA_R;  // B2

		if(statusAzul != 2 && statusVermelho == 16 && statusAmarelo == 4){
			ligaAzul();
			clicou = 1;
			saida = 0;
		}else if(statusAzul == 2 && statusVermelho != 16 && statusAmarelo == 4){
			ligaVermelho();
			clicou = 1;
			saida = 1;
		}else if(statusAzul == 2 && statusVermelho == 16 && statusAmarelo != 4){
			ligaAmarelo();
			clicou = 1;
			saida = 2;
		}else if(statusAzul == 2 && statusVermelho == 16 && statusAmarelo == 4)
			clicou = 0;
		else{
			clicou = 1;
			saida = 3;
		}
	}
	return saida;
}

int main(void) {
	//registradores A, B, D, E, F
	SYSCTL_RCGCGPIO_R = 0x3B;

	//LEDS PORTD
	GPIO_PORTD_DIR_R = 0x0F;
    GPIO_PORTD_DATA_R = 0x00;
	GPIO_PORTD_DEN_R = 0x0F;

	// BOTÃO AZUL
	GPIO_PORTE_DEN_R = 0x02;
	GPIO_PORTE_PUR_R = 0x02;
	// BOTÃO VERMELHO
	GPIO_PORTA_DEN_R = 0x10;
	GPIO_PORTA_PUR_R = 0x10;
	// BOTÃO AMARELO
	GPIO_PORTB_DEN_R = 0x04;
	GPIO_PORTB_PUR_R = 0x04;

	uint32_t vetor[20] = { 1, 2, 0, 0, 1, 0, 2, 1, 0, 2, 0, 1, 2, 1, 2, 1, 2, 0, 2, 0};
	uint32_t nivel = 1;

	uint32_t i = 0;

	uint32_t X = 1;
	uint32_t M = 9;
	uint32_t a = 65;
	uint32_t c = 27;

	/*ligaAzul();
	ligaVermelho();
	ligaAmarelo();
	perdeu();*/
	venceu();

	for(i = 0; i < 20; i++){
		X=(a * X + c) % M;
		if (X < 3)
		{
			vetor[i] = 0;
		}
		else if (X >= 3 && X < 6)
		{
			vetor[i] = 1;
		}
		else
		{
			vetor[i] = 2;
		}
	}

	while(1){

		for(i = 0; i < nivel; i++){
			if(vetor[i] == 0){
				ligaAzul();
			}else if(vetor[i] == 1){
				ligaVermelho();
			}else if(vetor[i] == 2){
				ligaAmarelo();
			}
		}

		for(i = 0; i < nivel;i++){
			uint32_t resposta = esperaBotao();
			if(resposta != vetor[i]){
				perdeu();
				nivel = 0;

				// Aleatório
				for(i = 0; i < 20; i++){
					X=(a * X + c) % M;
					if (X < 3)
					{
						vetor[i] = 0;
					}
					else if (X >= 3 && X < 6)
					{
						vetor[i] = 1;
					}
					else
					{
						vetor[i] = 2;
					}
				}
			}
		}

		uint32_t delay = 0;
		for(delay = 0; delay < tempoHigh; delay++);

		nivel++;
		if(nivel == 21){
			venceu();
			nivel = 1;
		}

	}

	return 0;
}
