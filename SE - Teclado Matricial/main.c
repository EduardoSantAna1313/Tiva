#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>
#include "UART.h"

/* LINHA OUTPUT PA4, PA5, PA6, PA7
 * COLUNA INPUT PE0, PE1, PE2, PE3
 */

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

volatile uint32_t linha = 0;

uint32_t lerColuna(){
	uint32_t coluna = GPIO_PORTE_DATA_R;
	if(((coluna>>3) & 0x01) == 1){
		return 4;
	}else if(((coluna>>2) & 0x01) == 1){
		return 3;
	}else if(((coluna>>1) & 0x01) == 1){
		return 2;
	}else if( (coluna & 0x01) == 1){
		return 1;
	}
	return 0;
}

void interrupcaoMatriz(void){
	uint32_t coluna = lerColuna();
	if(linha == 1){
		if(coluna == 1){
			uartEscrever("\n1");
		}else if(coluna== 2){
			uartEscrever("\n2");
		}else if(coluna == 3){
			uartEscrever("\n3");
		}else if(coluna== 4){
			uartEscrever("\nA");
		}
	}else if (linha == 2){
		if(coluna == 1){
			uartEscrever("\n4");
		}else if(coluna == 2){
			uartEscrever("\n5");
		}else if(coluna == 3){
			uartEscrever("\n6");
		}else if(coluna == 4){
			uartEscrever("\nB");
		}
	}else if (linha == 3){
		if(coluna == 1){
			uartEscrever("\n7");
		}else if(coluna == 2){
			uartEscrever("\n8");
		}else if(coluna == 3){
			uartEscrever("\n9");
		}else if(coluna == 4){
			uartEscrever("\nC");
		}
	}else if (linha == 4){
		if(coluna == 1){
			uartEscrever("\n*");
		}else if(coluna == 2){
			uartEscrever("\n0");
		}else if(coluna == 3){
			uartEscrever("\n#");
		}else if(coluna == 4){
			uartEscrever("\nD");
		}
	}
	GPIO_PORTE_ICR_R = PIN0 | PIN1 | PIN2 | PIN3;
}

void IntUART(void){
	//uartEscrever("\n interrupcao");
}

void delay(uint32_t n){
	uint32_t cont = 0;
	for(cont = 0; cont < n ; cont++);
}

int main(void) {
	SYSCTL_RCGCGPIO_R = PORTA | PORTE | PORTF;

	GPIO_PORTA_DIR_R = PIN4 | PIN5 | PIN6 | PIN7;
	GPIO_PORTA_DEN_R = PIN4 | PIN5 | PIN6 | PIN7;
	GPIO_PORTA_DATA_R = 0x00;

	GPIO_PORTE_DEN_R = PIN0 | PIN1 | PIN2 | PIN3;
	GPIO_PORTE_DATA_R = 0x00;
	GPIO_PORTE_PUR_R = PIN0 | PIN1 | PIN2 | PIN3;
	GPIO_PORTE_ICR_R = PIN0 | PIN1 | PIN2 | PIN3;
	GPIO_PORTE_IM_R = PIN0 | PIN1 | PIN2 | PIN3;
	NVIC_EN0_R = 1 << 4;

	GPIO_PORTF_DIR_R = PIN1 | PIN2 | PIN3;
	GPIO_PORTF_DEN_R = PIN1 | PIN2 | PIN3;
	GPIO_PORTF_DATA_R = 0x00;

	inicializarUART();
	uartEscrever("\nTESTE");
	while(1){
		GPIO_PORTA_DATA_R = PIN4;
		linha = 1;
		delay(1000);

		GPIO_PORTA_DATA_R = PIN5;
		linha = 2;
		delay(1000);

		GPIO_PORTA_DATA_R = PIN6;
		linha = 3;
		delay(1000);

		GPIO_PORTA_DATA_R = PIN7;
		linha = 4;
		delay(100);
	}

	return 0;
}
