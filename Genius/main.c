//PD0 - azul
//PD1 - vermelho
//PD2 - amarelo

//PE1 - botao azul
//PE2 - botao vermelho
//PE3 - botao amarelo

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include <inc/tm4c123gh6pm.h>

#define tempoHigh 1000000
#define tempoLow 1000000

void venceu(){
	volatile uint32_t ui32Loop;
	int i;

	for(i = 0; i <5; i++){
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2, 0x07);
		for(ui32Loop = 0; ui32Loop < tempoHigh; ui32Loop++)
		{
		}
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2, 0x00);
		for(ui32Loop = 0; ui32Loop < tempoLow; ui32Loop++)
		{
		}
	}

}

void perdeu(){
	volatile uint32_t ui32Loop;
	int i ;

	for(i = 0; i < 5 ; i ++){
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x02);
		for(ui32Loop = 0; ui32Loop < tempoHigh; ui32Loop++);
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
		for(ui32Loop = 0; ui32Loop < tempoLow; ui32Loop++);
	}

}

void ligaAzul(){
	volatile uint32_t ui32Loop;
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x01);
	for(ui32Loop = 0; ui32Loop < tempoHigh; ui32Loop++);
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x00);
	for(ui32Loop = 0; ui32Loop < tempoLow; ui32Loop++);
}

void ligaVermelho(){
	volatile uint32_t ui32Loop;
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x02);
	for(ui32Loop = 0; ui32Loop < tempoHigh; ui32Loop++);
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
	for(ui32Loop = 0; ui32Loop < tempoLow; ui32Loop++);
}

void ligaAmarelo(){
	volatile uint32_t ui32Loop;
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0x04);
	for(ui32Loop = 0; ui32Loop < tempoHigh; ui32Loop++);
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0x00);
	for(ui32Loop = 0; ui32Loop < tempoLow; ui32Loop++);
}

int esperaBotao(){
	int saida = 0;

	uint32_t valorBotaoAzul;
	uint32_t valorBotaoVermelho;
	uint32_t valorBotaoAmarelo;

	int clicou = 0;

	while(clicou == 0){
		valorBotaoAzul = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1);
		valorBotaoVermelho = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2);
		valorBotaoAmarelo = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3);

		if(valorBotaoAzul == GPIO_PIN_1){
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x00);
		}else{
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x01);
			ligaAzul();
			clicou = 1;
			saida = 0;
		}

		if(valorBotaoVermelho == GPIO_PIN_2){
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
		}else{
			ligaVermelho();
			clicou = 1;
			saida = 1;
		}

		if(valorBotaoAmarelo == GPIO_PIN_3){
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0x00);
		}else{
			ligaAmarelo();
			clicou = 1;
			saida = 2;
		}
	}
	return saida;
}

int main(void) {
	int vetor[20];
	vetor[0] = 0;
	vetor[1] = 0;
	vetor[2] = 2;
	vetor[3] = 1;
	vetor[4] = 1;
	vetor[5] = 2;
	vetor[6] = 1;
	vetor[7] = 0;
	vetor[8] = 2;
	vetor[9] = 2;
	vetor[10] = 1;
	vetor[11] = 2;
	vetor[12] = 0;
	vetor[13] = 1;
	vetor[14] = 0;
	vetor[15] = 2;
	vetor[16] = 0;
	vetor[17] = 1;
	vetor[18] = 1;
	vetor[19] = 2;

	int nivel = 1;

	// BOTOES
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	// LEDS PLACA
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

	venceu();

	while(1){

		int i = 0;

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
			int resposta = esperaBotao();
			if(resposta != vetor[i]){
				perdeu();
				nivel = 0;
			}
		}

		int delay = 0;
		for(delay = 0; delay < tempoHigh; delay++);

		nivel++;
		if(nivel == 21){
			venceu();
			nivel = 1;
		}

	}

	return 0;
}
