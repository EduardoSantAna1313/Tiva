/*
 *PINO E3 - SENSOR DE TEMPERATURA
 *PINO D0 - PWM
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "driverlib/pwm.h"

/*void configuraUART(){
	//configurar o clock...
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	//habilitar a uart que será utilizada
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    //configurar os pinos que serão utilizados por esta uart
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    //configurar o data frame
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}
*/
void configuraPWM(void) {

	//Configura o clock do pwm  dividindo o clock do sistema por 8
	SysCtlPWMClockSet(SYSCTL_PWMDIV_8);

	// habilita a porta D...
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	// habilita o pwm1 para usar com os leds da placa
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

	//Determina os pinos da porta D que serão usados como pwm
	GPIOPinConfigure(GPIO_PD0_M1PWM0);
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);

	//Congurando as opçoes dos pwms
	//PWM_GEN_0 => M1PWM0 and M1PWM1
	PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, 10000);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 10000/2);

	// Ligando o gerador propriamente dito
	PWMGenEnable(PWM1_BASE, PWM_GEN_0);

	// Ligando os pinos de saida
	PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);



}

void setFrequencia(int frequencia){
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, frequencia);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, frequencia/2);
}

void configuraADC(){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	// habilita o ADC0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

	//deixamos a taxa de amostragem no máximo...
	//configuramos a sequencia de amostragem
	ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);

	// todas os três passos da amostragem vão acessar o sensor de temperatura

	// define pino E1 como ADC
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH2);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH2);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH2);
	//habilitamos a interupção, IE e indicamos que este é o último passo da sequência de amostragem
	ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_CH2 | ADC_CTL_IE | ADC_CTL_END);

	//habilitamos o ADC0
	ADCSequenceEnable(ADC0_BASE, 1);
}

int main(void) {
	uint32_t ui32ADC0Value[4];
	double temp;

	SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	configuraADC();
	//configuraPWM();
	//configuraUART():

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	GPIO_PORTB_DIR_R = GPIO_PIN_2;
	GPIO_PORTB_DEN_R = GPIO_PIN_2;

	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x00);

	while (1) {
		//limpa a interrupção para garantir que nenhuma digitalização foi realizada
		ADCIntClear(ADC0_BASE, 1);

		//disparamos a digitalização via software
		ADCProcessorTrigger(ADC0_BASE, 1);

		//monitoramos a interrupção para saber quando as quatro amostragens estão prontas
		while (!ADCIntStatus(ADC0_BASE, 1, false)) {
		}

		//pegamos as quatro amostragens do adc e colocamos no nosso array.
		ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);

		temp = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3]) / 4;
		temp = temp*150/2047;
		if(temp > 26){
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2);
		}else{
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x00);
		}

		SysCtlDelay(50000000);
	}
}
