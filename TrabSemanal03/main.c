#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pwm.h"
#include <inc/tm4c123gh6pm.h>

// interrupção ativada qnd um caracter é mandado
void UARTIntHandler(void)
{
    uint32_t ui32Status;

    ui32Status = UARTIntStatus(UART0_BASE, true);
    // limpa a interrupção
    UARTIntClear(UART0_BASE, ui32Status);

    while(UARTCharsAvail(UART0_BASE)) // enquanto o UART0 receber um caracter
    {
    	char c = UARTCharGet(UART0_BASE);

    	if(c == 'e'){
    		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
    		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);//led azul
    		frente();

    	}else if(c == 'f'){
    		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
    		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); // led vermelho
    		tras();
    	}else if(c == 'g'){
    		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
    		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);//led verde
    		direita();
    	}else if(c == 'h'){
    		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
    		GPIOPinWrite(GPIO_PORTF_BASE,  GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,  GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    		esquerda();
    	}else if(c == 'i'){
    		PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, false);
    		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
    	}else if(c == '0'){
    		setVelocidade(40);
    	}else if(c == '1'){
    		setVelocidade(80);
    	}else if(c == '2'){
    		setVelocidade(120);
    	}else if(c == '3'){
    		setVelocidade(160);
    	}else if(c == '4'){
    		setVelocidade(200);
    	}else if(c == '5'){
    		setVelocidade(240);
    	}else if(c == '6'){
    		setVelocidade(280);
    	}else if(c == '7'){
    		setVelocidade(320);
    	}else if(c == '8'){
    		setVelocidade(360);
    	}else if(c == '9'){
    		setVelocidade(400);
    	}
    	// envia o caracter recebido a uma porta especifica
        UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE));

    }
}

void configuraPWM(void) {

	//Configura o clock do pwm  dividindo o clock do sistema por 1
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

	// habilita a porta D... lá vamos nós de led novamente
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	// habilita o pwm1 para usar com os leds da placa
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

	//Determina os pinos da porta D que serão usados como pwm
	GPIOPinConfigure(GPIO_PD0_M1PWM0);
	GPIOPinConfigure(GPIO_PD1_M1PWM1);
	//observe que estamos usandos os macros padronizados para acesso aos pinos
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	//Congurando as opçoes dos pwms
	//PWM_GEN_0 => M1PWM0 and M1PWM1

	PWMGenConfigure(PWM1_BASE, PWM_GEN_0,
			PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

	//determinando os períodos baseados em clocks
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, 400);

	//configurando o duty Cicle
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 400);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, 400);

	// Ligando o gerador propriamente dito
	PWMGenEnable(PWM1_BASE, PWM_GEN_0);

	// Ligando os pinos de saida
	PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);

}

void frente(void){
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0x00);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x02);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x04);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x00);
}

void tras(void){
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0x01);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x00);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x00);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x08);
}

void esquerda(void){
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0x00);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x00);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x04);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x00);
}

void direita(void){
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0x00);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x02);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x00);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x00);
}

void setVelocidade(int velocidade){
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, velocidade);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, velocidade);
}

int main(void) {

	// habilita e configura o clock
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	//habilitar a uart que será utilizada
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    //configurar os pinos que serão utilizados por esta uart
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // habilita o pino F2, LED azul da placa.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    //configurar o data frame
    // endereço da porta, baud rate, etc...
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    IntMasterEnable();
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    configuraPWM();

    // habilita GPIO portB
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIO_PORTB_DIR_R = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_PORTB_DEN_R = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;

    while (1)
    {
    }

}
