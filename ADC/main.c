#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"

int main(void)
{
	//Array para armazenar dados vindos do fifo do adc profundidade 4
	uint32_t ui32ADC0Value[4];

	//Para armazenar as temperaturas lidas
	volatile uint32_t ui32TempAvg;
	volatile uint32_t ui32TempValueC;
	volatile uint32_t ui32TempValueF;

	// habilita e configura o clock
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

	// habilita o ADC0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

	//deixamos a taxa de amostragem no máximo...
	//configuramos a sequencia de amostragem
	ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);

	// todas os três passos da amostragem vão acessar o sensor de temperatura
	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);

	//habilitamos a interupção, IE e indicamos que este é o último passo da sequência de amostragem
	ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);

	//habilitamos o ADC0
	ADCSequenceEnable(ADC0_BASE, 1);

	while(1)
	{
	   //limpa a interrupção para garantir que nenhuma digitalização foi realizada
       ADCIntClear(ADC0_BASE, 1);

       //disparamos a digitalização via software
       ADCProcessorTrigger(ADC0_BASE, 1);

       //monitoramos a interrupção para saber quando as quatro amostragens estão prontas
	   while(!ADCIntStatus(ADC0_BASE, 1, false))
	   {
	   }

	   //pegamos as quatro amostragens do adc e colocamos no nosso array.
	   ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);
	   //dispensa apresentações
	   ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;
	   ui32TempValueC = (1475 - ((2475 * ui32TempAvg)) / 4096)/10;
	   ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;
	}
}
