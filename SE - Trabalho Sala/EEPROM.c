#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>

#define endereco1 (*((volatile uint32_t*)0x20000000)) // EEPROM começa em 0x20000000
#define endereco2 (*((volatile uint32_t*)0x20000020))
#define EEPROM_INIT_OK 0
#define EEPROM_INIT_ERROR 2

uint32_t incializarEEPROM(void)
{
    uint32_t ui32Aux;
    SYSCTL_RCGCEEPROM_R = 1;
    for(ui32Aux=0;ui32Aux<2;ui32Aux++);
    while(EEPROM_EEDONE_WORKING == 0);
    if(EEPROM_EESUPP_R == 8 || EEPROM_EESUPP_R == 4)
    {
        return(EEPROM_INIT_ERROR);
    }

    SYSCTL_SREEPROM_R = 1;
    SYSCTL_SREEPROM_R = 0;
    for(ui32Aux=0;ui32Aux<2;ui32Aux++);
    while(EEPROM_EEDONE_WORKING == 0);
    if(EEPROM_EESUPP_R == 8 || EEPROM_EESUPP_R == 4)
    {
    	return(EEPROM_INIT_ERROR);
    }

    return(EEPROM_INIT_OK);
}

uint32_t getMemoria(uint32_t n){
	if(n == 0){
		return endereco1;
	}
}
