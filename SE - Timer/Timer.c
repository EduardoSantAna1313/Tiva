#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>

void initTimer_INT(void){
	NVIC_ST_RELOAD_R = 2000000;
	NVIC_ST_CTRL_R = NVIC_ST_CTRL_INTEN | NVIC_ST_CTRL_ENABLE;
}

// declarar função da interrupção no startup
void TIMER_INT(){
	GPIO_PORTF_DATA_R ^= 0x02;
	NVIC_ST_RELOAD_R = 2000000;
}

void initTimer(void){
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 4000000;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 0x05;
}

void wait(uint32_t delay){
  volatile uint32_t elapsedTime;
  uint32_t startTime = NVIC_ST_CURRENT_R;
  do{
    elapsedTime = (startTime-NVIC_ST_CURRENT_R)&0x00FFFFFF;
  }
  while(elapsedTime <= delay);
}
