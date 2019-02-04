#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>

int main(void) {
	SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R5;

	// Led F1
	GPIO_PORTF_DIR_R = 0x02;
	GPIO_PORTF_DEN_R = 0x12;
	GPIO_PORTF_DATA_R = 0x00;

	// botao F4
	GPIO_PORTF_PUR_R = 0x10;

	uint32_t status = 0;

	while(1){
		status = GPIO_PORTF_DATA_R;
		if(status != 0x10){
			GPIO_PORTF_DATA_R ^= 0x02;
		}
	}


	return 0;
}

