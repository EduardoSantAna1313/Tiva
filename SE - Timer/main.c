#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>

int main(void) {
	SYSCTL_RCGCGPIO_R = 0x20;

	//LED
	GPIO_PORTF_DIR_R = 0x02;
	GPIO_PORTF_DATA_R = 0x00;
	GPIO_PORTF_DEN_R = 0x02;

	// sem interrupção
	//initTimer();

	// com interrupção
	initTimer_INT();

	while(1){
		__asm("WFI");
	}
	return 0;
}

