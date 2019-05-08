#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

void setFrequencia(int frequencia){
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, frequencia);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, frequencia/2);
}

void UARTIntHandler(void) {
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART0_BASE, true);

	UARTIntClear(UART0_BASE, ui32Status);

	while(UARTCharsAvail(UART0_BASE))
	{
		char c = UARTCharGet(UART0_BASE);
		if(c == 'A'){
			setFrequencia(23848);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
		}else if ( c == 'B'){
			setFrequencia(22548);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
		}else if(c == 'C'){
			setFrequencia(21330);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
		}else if( c == 'D'){
			setFrequencia(20100);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
		}else if( c == 'E'){
			setFrequencia(18998);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
		}else if( c == 'F'){
			setFrequencia(17900);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
		}else if( c == 'G'){
			setFrequencia(16888);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
		}else if( c == 'H'){
			setFrequencia(15938);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
		}else if( c == 'I'){
			setFrequencia(15062);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
		}else if( c == 'J'){
			setFrequencia(14200);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
		}else if( c == 'K'){
			setFrequencia(13400);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
		}else if( c == 'L'){
			setFrequencia(12648);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
		}else if (c == 'N'){
			PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, 12000);
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 1);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
		}
		UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE));
	}

}



void configuraUART() {
	// habilita e configura o clock
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	//habilitar a uart que será utilizada
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	//configurar os pinos que serão utilizados por esta uart
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	//configurar o data frame
	// endereço da porta, baud rate, etc...
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	IntMasterEnable();
	IntEnable(INT_UART0);
	UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}


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

	//determinando os períodos baseados em clocks
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, 400);

	//configurando o duty Cicle
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 200);

	// Ligando o gerador propriamente dito
	PWMGenEnable(PWM1_BASE, PWM_GEN_0);

	// Ligando os pinos de saida
	PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);

}

int main(void) {
	configuraUART();
	configuraPWM();

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

	setFrequencia(20000);

	while(1){
	}
	return 0;
}
