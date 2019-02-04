#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>

void MinhaInterrupcao(void){
	GPIO_PORTF_ICR_R = 0x10;
	GPIO_PORTF_DATA_R ^= 0x02;
}

int main(void) {
	SYSCTL_RCGCGPIO_R = 0x20;

	GPIO_PORTF_DIR_R |= 0x02;
	GPIO_PORTF_DATA_R |= 0x00;
	GPIO_PORTF_DEN_R |= 0x12;

	//GPIO_PORTF_DEN_R = 0x10;
	GPIO_PORTF_PUR_R = 0x10;
	GPIO_PORTF_ICR_R = 0x10;
	//GPIO_PORTF_IBE_R = 0x10; // BOTH EDGES
	GPIO_PORTF_IM_R = 0x10;
	NVIC_EN0_R = 1 << 30;

	while(1){
		__asm("WFI");
	}

	return 0;
}
