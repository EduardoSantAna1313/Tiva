#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>

void inicializarTimer(void){
	NVIC_ST_RELOAD_R = 4000000;
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_INTEN | NVIC_ST_CTRL_ENABLE;
}
