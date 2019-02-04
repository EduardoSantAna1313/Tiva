#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>

#define SYSCTL_RCGCGPIO_PORTA (*((volatile uint32_t*)0x43FCC100))
#define SYSCTL_RCGCGPIO_PORTB (*((volatile uint32_t*)0x43FCC104))
#define SYSCTL_RCGCGPIO_PORTC (*((volatile uint32_t*)0x43FCC108))
#define SYSCTL_RCGCGPIO_PORTD (*((volatile uint32_t*)0x43FCC10C))
#define SYSCTL_RCGCGPIO_PORTE (*((volatile uint32_t*)0x43FCC110))
#define SYSCTL_RCGCGPIO_PORTF (*((volatile uint32_t*)0x43FCC114))

#define DATA_PORTD_BIT0 (*((volatile uint32_t*)0x420E7F80))
#define DATA_PORTD_BIT1 (*((volatile uint32_t*)0x420E7F84))
#define DATA_PORTD_BIT2 (*((volatile uint32_t*)0x420E7F88))
	
int main(void){
	 SYSCTL_RCGCGPIO_PORTD = 1;
	 GPIO_PORTD_DIR_R = 0b11;
	 GPIO_PORTD_DATA_R = 0;
	 GPIO_PORTD_PUR_R = 1 << 2;
	 GPIO_PORTD_DEN_R = 0b111;
	 while(1){
		 if((GPIO_PORTD_DATA_R & (1 << 2)) != 0){
			 DATA_PORTD_BIT0 = 0;
			 DATA_PORTD_BIT1 = 0;
		}else{
			 DATA_PORTD_BIT0 = 1;
			 DATA_PORTD_BIT1 = 1;
		}
	}
}
