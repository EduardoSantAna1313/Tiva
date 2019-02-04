/*
 *************LCD**************
 VSS -> GND
 VDD -> 3.3V
 V0  -> GND
 RS  -> PA6
 R/W -> GND
 E   -> PA7
 D0  -> PB0
 ...
 D7  -> PB7

 ********INFRAVERMELHO*********
 PD0 e PD1

 ************UART*************
 PA0 e PA1

 ***********SWITCH***********
 PF4

 ***********LED PORTA**********
 PA4
 */

#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>
#include "UART.h"
#include "LCD.h"
#include "defines.h"

volatile uint32_t contadorCol = 0;
volatile uint32_t contadorLin = 0;
char senha[4];
char login[5];
volatile uint32_t sala = 1;
volatile uint32_t pessoas1 = 0;
volatile uint32_t pessoas2 = 0;
volatile uint32_t entrada = 0;

void imprimePessoas() {
	setCursor(0, 15);
	if (sala == 1) {
		printInt(pessoas1);
	} else if (sala == 2) {
		printInt(pessoas2);
	}

}

void imprimeMenu(void) {
	clear();
	setCursor(0, 0);
	printString("Sala ");
	printInt(sala);
	delay(30000);
	clear();
	setCursor(0, 0);
	printString("Login:");
	setCursor(1, 0);
	printString("Senha:");
	imprimePessoas();
}

bool comparar(char *c1, char *c2) {
	while (*c1) {
		if (*c1 == *c2) {
			c1++;
			c2++;
		} else {
			return false;
		}
	}
	return true;
}

bool validarEntrada(char *login1, char *login2, char *senha1, char *senha2) {
	if (comparar(login1, login2) && comparar(senha1, senha2)) {
		return true;
	}
	return false;
}

void piscarLed() {
	GPIO_PORTA_DATA_R |= PIN4;
	delay(25000);
	GPIO_PORTA_DATA_R &= 0x11110111;
}

void UART_INT(void) {
	char ch = uartLer();

	if (ch >= '0' && ch <= '9') {

		if (contadorLin == 0) {
			setCursor(0, 6 + contadorCol);
			printChar(ch);
			login[contadorCol] = ch;
			contadorCol++;
			if (contadorCol == 5) {
				contadorCol = 0;
				contadorLin = 1;
				setCursor(1, 6);
			}
		} else if (contadorLin == 1) {
			setCursor(1, 6 + contadorCol);
			//printChar(ch);
			printChar('*');
			senha[contadorCol] = ch;
			contadorCol++;
			if (contadorCol == 4) {
				contadorCol = 0;
				contadorLin = 0;
				if (sala == 1 && pessoas1 == CAPACIDADE) {
					ch = ' ';
					clear();
					printString("Sala Cheia");
					delay(50000);
					imprimeMenu();
				} else if (sala == 2 && pessoas2 == CAPACIDADE) {
					ch = ' ';
					clear();
					printString("Sala Cheia");
					delay(50000);
					imprimeMenu();
				} else {
					bool entrar = validarEntrada(LOGIN, login, SENHA, senha);
					delay(10000);
					if (entrar) {
						clear();
						setCursor(0, 0);
						printString("Entrada");
						setCursor(1, 0);
						printString("Permitida");
						GPIO_PORTA_DATA_R |= PIN4;
						delay(50000);
						imprimeMenu();
						entrada = 1;
					} else {
						clear();
						setCursor(0, 0);
						printString("Senha ou Login");
						setCursor(1, 0);
						printString("invalido");
						delay(50000);
						imprimeMenu();
						entrada = 0;
					}
				}

			}

		}
	}
}

void inicializarADC() {
	SYSCTL_RCGCADC_R |= 3;

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
}

void inicializarSwitch(void) {
	GPIO_PORTF_DEN_R |= PIN4;
	GPIO_PORTF_PUR_R |= PIN4;
	GPIO_PORTF_ICR_R |= PIN4;
	GPIO_PORTF_IM_R |= PIN4;
	NVIC_EN0_R |= 1 << 30;
}

void PORTF_INT(void) {
	uint32_t sw = (GPIO_PORTF_DATA_R >> 4) & 1;
	if (sw == 1) {
		clear();
		if (sala == 1) {
			sala = 2;
		} else {
			sala = 1;
		}
		imprimeMenu();
	}
	GPIO_PORTF_ICR_R = PIN4;
}

void inicializarLed() {
	GPIO_PORTA_DIR_R |= PIN4;
	GPIO_PORTA_DEN_R |= PIN4;
	GPIO_PORTA_DATA_R &= 0b11110111;
}

int main(void) {
	SYSCTL_RCGCGPIO_R = PORTA | PORTB | PORTD | PORTF;
	inicializarLCD();
	inicializarADC();
	inicializarUART();
	inicializarSwitch();
	inicializarLed();

	uint32_t leituraAtual1 = 0;
	uint32_t leituraAtual2 = 0;
	uint32_t vetorSentido[2] = { 0, 0 };
	uint32_t contVetorSentido = 0;

	imprimeMenu();
// teste
	while (1) {

		if (entrada == 1) {
			leituraAtual1 = ADC0_SSFIFO0_R;
			leituraAtual2 = ADC1_SSFIFO0_R;

			if (leituraAtual1 > NIVEL && leituraAtual2 < NIVEL) {
				while (leituraAtual1 > NIVEL) {
					leituraAtual1 = ADC0_SSFIFO0_R;
				}
				vetorSentido[contVetorSentido] = 1;
				contVetorSentido++;
			} else if (leituraAtual2 > NIVEL && leituraAtual1 < NIVEL) {
				while (leituraAtual2 > NIVEL) {
					leituraAtual2 = ADC1_SSFIFO0_R;
				}
				vetorSentido[contVetorSentido] = 2;
				contVetorSentido++;
			} else {
				contVetorSentido = contVetorSentido;
			}
			if (contVetorSentido == 2) {
				if (vetorSentido[0] == 2 && vetorSentido[1] == 1) {
					// Saindo
					if (sala == 1 && pessoas1 > 0) {
						pessoas1--;
					} else if (sala == 2 && pessoas2 > 0) {
						pessoas2--;
					}
					imprimePessoas();
				} else if (vetorSentido[0] == 1 && vetorSentido[1] == 2) {
					//Entrando
					if (sala == 1 && pessoas1 < CAPACIDADE) {
						pessoas1++;
						GPIO_PORTA_DATA_R &= 0b11110111;
					} else if (sala == 2 && pessoas2 < CAPACIDADE) {
						pessoas2++;
						GPIO_PORTA_DATA_R &= 0b11110111;
					}
					imprimePessoas();
				}
				contVetorSentido = 0;
			}
		}
	}
	return 0;
}
