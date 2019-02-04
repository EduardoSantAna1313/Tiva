#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>
#include "Timer.h"
#include "UART.h"
#include "Defines.h"

volatile uint32_t FREQUENCY = 100;
volatile uint32_t AMPLITUDE = 255;
volatile char Type = 'Q';

const uint8_t SEN[32] = {128, 153, 178, 201, 220, 236, 248, 254, 255, 251, 242, 229, 211, 190, 166, 140, 115, 89, 65, 44, 26, 13, 4, 0, 1, 7, 19, 35, 54, 77, 102, 128};
const uint8_t sen60[60] = {128 ,141 ,154 ,167 ,180 ,192 ,203 ,213 ,223 ,231 ,238 ,244 ,249 ,253 ,254 ,255 ,254 ,253 ,249 ,244 ,238 ,231 ,223 ,213 ,203 ,192 ,180 ,167 ,154 ,141,128 ,114 ,101 ,88 ,75 ,64 ,52 ,42 ,32 ,24 ,17 ,11 ,6 ,2 ,0 ,0 ,0 ,2 ,6 ,11 ,17 ,24 ,32 ,42 ,52 ,64 ,75 ,88 ,101 ,114};

volatile uint8_t indiceS = 0;
volatile uint8_t indiceD = 0;
volatile uint8_t indiceQ = 0;
volatile uint8_t indiceT = 0;
volatile int32_t triang = 0;
void interrupcaoTimer(){

	if(Type == 'Q'){
		if(indiceQ == 0){
			GPIO_PORTB_DATA_R = AMPLITUDE;
			indiceQ = 1;
		}else{
			GPIO_PORTB_DATA_R &= 0;
			indiceQ = 0;
		}

		NVIC_ST_RELOAD_R = 4000000/(2 * FREQUENCY);
	}else if(Type == 'T'){
		uint8_t valor = triang*AMPLITUDE/255;
		GPIO_PORTB_DATA_R = valor;
		if(indiceT == 0){
			if(FREQUENCY < 1000){
				triang += 4;
				if(triang >= 253){
					triang = 255;
				}
				NVIC_ST_RELOAD_R = 4000000/(128 * FREQUENCY);
			}else{
				triang += 16;
				if(triang >= 241){
					triang = 255;
				}
				NVIC_ST_RELOAD_R = 4000000/(32 * (FREQUENCY+100));
			}

			if(triang >= 247){
				indiceT = 1;
			}
		}else{
			if(FREQUENCY < 1000){
				triang -= 4;
				if(triang < 5){
					triang = 0;
				}
				NVIC_ST_RELOAD_R = 4000000/(128 * FREQUENCY);
			}else{
				triang -= 16;
				if(triang < 15){
					triang = 0;
				}
				NVIC_ST_RELOAD_R = 4000000/(32 * (FREQUENCY+100));
			}
			if(triang <= 0){
				indiceT = 0;
			}
		}

	}else if(Type == 'S'){
		if(FREQUENCY < 2500){
			GPIO_PORTB_DATA_R = sen60[indiceS] * AMPLITUDE/255;
			indiceS ++;
			if(indiceS > 59){
				indiceS = 0;
			}
			NVIC_ST_RELOAD_R = 4000000/(60 * FREQUENCY);
		}else{
			GPIO_PORTB_DATA_R = SEN[indiceS] * AMPLITUDE/255;
			indiceS ++;
			if(indiceS > 31){
				indiceS = 0;
			}
			NVIC_ST_RELOAD_R = 4000000/(32 * (FREQUENCY+100));
		}
	}else if(Type == 'D'){
		GPIO_PORTB_DATA_R = indiceD*AMPLITUDE/255;

		if(FREQUENCY < 2000){
			indiceD += 4;
			NVIC_ST_RELOAD_R = 4000000/(64 * FREQUENCY);
		}else{
			indiceD += 8;
			NVIC_ST_RELOAD_R = 4000000/(32 * (FREQUENCY+100));
		}

		if(indiceD >= 255){
			indiceD = 0;
		}

	}
}

void piscarLED(uint32_t id) {
	uint32_t cont = 0;
	GPIO_PORTF_DATA_R |= id;
	for (cont = 0; cont < 100000; cont++);
	GPIO_PORTF_DATA_R &= 0x00;
	for (cont = 0; cont < 100000; cont++);
}

bool compara(char *c1, char *c2) {
	while (*c2) {
		if (*c1 == *c2) {
			c1++;
			c2++;
		} else {
			return false;
		}
	}
	return true;
}

uint8_t atoi(char c){
	if(c == '0'){
		return 0;
	}else if(c == '1'){
		return 1;
	}else if(c == '2'){
		return 2;
	}else if(c == '3'){
		return 3;
	}else if(c == '4'){
		return 4;
	}else if(c == '5'){
		return 5;
	}else if(c == '6'){
		return 6;
	}else if(c == '7'){
		return 7;
	}else if(c == '8'){
		return 8;
	}else if(c == '9'){
		return 9;
	}else{
		return 0;
	}
}


uint32_t pow(uint32_t b, uint32_t e){
	uint32_t r = 1;
	uint32_t i = 0;
	if(e == 0){
		return 1;
	}else{
		for(i = 0; i < e; i++){
			uint32_t aux = b*r;
			r = aux;
		}
	}
	return r;
}

uint32_t chat2int(char *p){
	uint32_t tam = 1;
	uint32_t result = 0;
	while(*p){
		result += atoi(*p)*10000/pow(10,tam);
		p++;
		tam++;
	}
	if(tam <= 5){
		return result/10;
	}else{
		return result;
	}

}

void interrupcaoUART(void) {
	char ch = ler();
	if(ch == 'S' || ch == 'T' || ch == 'Q' || ch == 'D'){
		char vetor[9];

		vetor[0] = ch;

		uint8_t i = 1;
		while(i < 9){
			ch = ler();
			if( (ch >= '0' && ch <= '9') || (ch >= 'A' || ch <= 'Z')){
				vetor[i] = ch;
				i++;
			}
		}

		Type = vetor[0];
		if(Type == 'Q'){
			GPIO_PORTB_DATA_R = AMPLITUDE;
		}
		char auxA[3];
		for(i = 1; i < 4; i++){
			auxA[i-1] = vetor[i];
		}

		AMPLITUDE = chat2int(auxA)/10;
		if(AMPLITUDE > 255){
			AMPLITUDE = 255;
		}

		char auxF[4];
		for(i = 4; i < 8; i++){
			auxF[i-4] = vetor[i];
		}

		FREQUENCY = chat2int(auxF);
		if(FREQUENCY > 5000){
			FREQUENCY = 5000;
		}
		if(compara(vetor, "S1001000") == 1){
			GPIO_PORTF_DATA_R ^= 0x02;
		}
	}
}

void delay(uint32_t ms){
	int i = 0;
	for(i = 0; i < ms; i++);
}

int main(void) {

// LED
	SYSCTL_RCGCGPIO_R |= PORTF;
	GPIO_PORTF_DIR_R  |= PIN1 + PIN2;
	GPIO_PORTF_DEN_R  |= PIN1 + PIN2;
	GPIO_PORTF_DATA_R |= 0x00;
// FIM LED

// DAC
	SYSCTL_RCGCGPIO_R |= PORTB;
	GPIO_PORTB_DIR_R  |= 0xFF;
	GPIO_PORTB_DEN_R  |= 0xFF;
	GPIO_PORTB_DATA_R  = 0x00;
// FIM DAC

	Type = 'Q';
	AMPLITUDE = 255;
	FREQUENCY = 10;
 	inicializarTimer();

	inicializarUART();

	delay(100);
	escreverString("INICIALIZANDO\n");

	while(1){
		__asm("WFI");
	}

	return 0;
}
