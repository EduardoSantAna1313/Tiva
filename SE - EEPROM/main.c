#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>

#define endereco (*((volatile uint32_t*)0x20000000))

#define EEPROM_INIT_OK 0
#define EEPROM_INIT_ERROR 2

uint32_t EInit(void)
{
    uint32_t ui32Aux;
    SYSCTL_RCGCEEPROM_R |= 1;
    for(ui32Aux=0;ui32Aux<2;ui32Aux++){}
    while(EEPROM_EEDONE_WORKING){}
    if(EEPROM_EESUPP_R == 8 || EEPROM_EESUPP_R == 4)
    {
        return(EEPROM_INIT_ERROR);
    }

    SYSCTL_SREEPROM_R = 1;
    SYSCTL_SREEPROM_R = 0;
    for(ui32Aux=0;ui32Aux<2;ui32Aux++){}
    while(EEPROM_EEDONE_WORKING){}
    if(EEPROM_EESUPP_R == 8 || EEPROM_EESUPP_R == 4)
    {
    	return(EEPROM_INIT_ERROR);
    }

    return(EEPROM_INIT_OK);

}

int main(void) {
	SYSCTL_RCGCGPIO_R = 0x20;

	GPIO_PORTF_DIR_R = 0x06;
	GPIO_PORTF_DEN_R = 0x06;
	GPIO_PORTF_DATA_R = 0x00;

	if(EInit()==0){
		while(1){
			//endereco = 20;
			if(endereco == 20){
				GPIO_PORTF_DATA_R = 0x02;
			}else{
				GPIO_PORTF_DATA_R = 0x04;
			}
		}
	}

	return 0;
}
