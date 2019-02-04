#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>

int main(void) {
	SYSCTL_RCGCGPIO_R = 32;

	 GPIO_PORTF_DIR_R = 14;
	 GPIO_PORTF_DATA_R = 0;
	 GPIO_PORTF_DEN_R = 31;

	 GPIO_PORTF_PUR_R = 17;

	 uint32_t status, cont;

	 while(1){
		 status = GPIO_PORTF_DATA_R;
		 cont = status >> 4;

		 if((status >> 1) != 1){
			 //GPIO_PORTF_DATA_R ^= 0x04;
		 }
	 }

	return 0;
}
