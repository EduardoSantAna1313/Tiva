#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.c"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.c"
#include "driverlib/gpio.h"

// Para trabalhar com os leds e bot�es
#define LED_PERIPH SYSCTL_PERIPH_GPIOF
#define LED_BASE GPIO_PORTF_BASE
#define RED_LED GPIO_PIN_1

#define Button_PERIPH SYSCTL_PERIPH_GPIOF
#define ButtonBase GPIO_PORTF_BASE
#define Button GPIO_PIN_4
#define ButtonInt GPIO_INT_PIN_4

volatile uint8_t value = 0;

// interrupt handler
void PortFIntHandler() {

	uint32_t status = 0;
	// Duas fun��es do grupo de gest�o de interrup��o de gpio pg.255
	status = GPIOIntStatus(ButtonBase, true); //pg261
	GPIOIntClear(ButtonBase, status); //pg.259
	uint32_t variavel = status>>4 & 1;
	if (variavel == 0) { // se entrar houve uma interrup��o
		GPIOPinWrite(LED_BASE, RED_LED, RED_LED); //pg. 286
		SysCtlDelay(7000000); //pg.493
		GPIOPinWrite(LED_BASE, RED_LED, 0x00); //pg. 286
	}

}

int main(void) {

	//Rodando a 80Mhz
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN| SYSCTL_XTAL_16MHZ);
	// para saber a frequencia SysCtlClockGet()

	// leds e but�es na mesma porta
	SysCtlPeripheralEnable(LED_PERIPH);
	SysCtlDelay(3);

	// configura��o do pino de entrada - completa
	GPIOPinTypeGPIOInput(ButtonBase, Button);
	GPIOPadConfigSet(ButtonBase, Button, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);

	// registrando o handler da interrup��o sem usar o startup_css.c
	GPIOIntRegister(GPIO_PORTF_BASE, PortFIntHandler);
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);

	// o led deve ser sa�da
	GPIOPinTypeGPIOOutput(LED_BASE, RED_LED);

	while (1) {

	}

}
