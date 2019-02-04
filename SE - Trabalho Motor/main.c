#include <inc/tm4c123gh6pm.h>
#include <inttypes.h>

#define PORTA 0x01
#define PORTB 0x02
#define PORTC 0x04
#define PORTD 0x08
#define PORTE 0x10
#define PORTF 0x20

#define PIN0 0x01
#define PIN1 0x02
#define PIN2 0x04
#define PIN3 0x08
#define PIN4 0x10
#define PIN5 0x20
#define PIN6 0x30
#define PIN7 0x40

#define PERIODO 25000
#define PASSO 22

char itoa(uint32_t num){
	if(num == 0){
		return '0';
	}else if(num == 1){
		return '1';
	}else if(num == 2){
		return '2';
	}else if(num == 3){
		return '3';
	}else if(num == 4){
		return '4';
	}else if(num == 5){
		return '5';
	}else if(num == 6){
		return '6';
	}else if(num == 7){
		return '7';
	}else if(num == 8){
		return '8';
	}else if(num == 9){
		return '9';
	}
	return 0;
}

void piscarLED(uint32_t id) {
	GPIO_PORTF_DATA_R ^= id;
}

volatile double RPM = 0;
volatile double posicao = 0;
volatile uint32_t clk = 0;
volatile uint32_t segundos = 0;
volatile uint32_t velocidadeAnterior = 50;
uint32_t status = 0;
volatile char vetor[2] = {'0' , '0'};
volatile uint8_t indiceVetor = 0;
volatile uint8_t sentido = 0;

//ENCODER
void InicializarEncoder(){
	SYSCTL_RCGCGPIO_R |= PORTE;
	GPIO_PORTE_DEN_R |= PIN2;
	GPIO_PORTE_PUR_R |= PIN2;
	GPIO_PORTE_ICR_R |= PIN2;
	GPIO_PORTE_IM_R  |= PIN2;
	NVIC_EN0_R = 1<<4;
}

void EncoderINT(void){
	uint32_t sig = (GPIO_PORTE_DATA_R >>2) & 0x01;

	if(sig == 1){
		while(sig==1){
			sig = (GPIO_PORTE_DATA_R >>2) & 0x01;
		}
		posicao++;
	}

	//GPIO_PORTE_ICR_R = PIN2;
}

// FIM ENCODER

// TIMER
void InicializarTimer(uint32_t ms){
	clk = ms*4000;
	NVIC_ST_RELOAD_R = clk;
	NVIC_ST_CTRL_R = NVIC_ST_CTRL_INTEN | NVIC_ST_CTRL_ENABLE;
}

void TimerINT(void){
	/*segundos++;

	if(segundos == 60){
		segundos = 0;
	}*/
	RPM = (int) (60 * (posicao/PASSO));

	uint32_t num1 = 0, num2 = 0;
	num1 = RPM/10;
	num2 = RPM;
	num2 = num2 % 10;


	Write(itoa(num2));
	Write('\n');

	posicao = 0;
	piscarLED(0x02);

	NVIC_ST_RELOAD_R = clk;
}
// FIM TIMER

// UART
void InicializarUART(){
	SYSCTL_RCGCUART_R |= PORTB;
	SYSCTL_RCGCGPIO_R |= PORTB;

	UART1_IBRD_R = 104; // 9600
	UART1_FBRD_R = 11;

	UART1_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);

	UART1_IFLS_R &= ~0x3F;
	UART1_IFLS_R += (UART_IFLS_TX1_8|UART_IFLS_RX1_8);

	UART1_IM_R |= (UART_IM_RXIM|UART_IM_TXIM|UART_IM_RTIM);
	UART1_CTL_R |= 0x01;

	GPIO_PORTB_LOCK_R 	|= 0x03;
	GPIO_PORTB_AFSEL_R 	|= 0x03;
	GPIO_PORTB_DEN_R 	|= 0x03;
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFFFF00)+0x00000011;
	NVIC_EN0_R = 1<<6;
}

void Write(char c) {
 	while ((UART1_FR_R & UART_FR_TXFF) != 0);
	UART1_DR_R = c;
}

char Read(void) {
	while ((UART1_FR_R & UART_FR_RXFE) != 0);
	return ((char) (UART1_DR_R & 0xFF));
}

void UART_INT(void) {
	char ch = Read();
	if(ch == 'H'){
		sentido = 0;
		PWM0_ENABLE_R = 0x01;
	}else if(ch == 'A'){
		sentido = 1;
		PWM0_ENABLE_R = 0x02;
	}else if(ch >= '0' && ch <= '9'){
		vetor[indiceVetor] = ch;
		indiceVetor++;
		if(indiceVetor == 2){
			uint32_t inteiro = (vetor[0] % 48) * 10;
			inteiro += (vetor[1] % 48);
			indiceVetor = 0;

			PWM0_0_CMPA_R = (PERIODO * inteiro) / 100;
			if(sentido == 0){
				PWM0_ENABLE_R = 0x01;
			}else if(sentido == 1){
				PWM0_ENABLE_R = 0x02;
			}
			//PWM0_ENABLE_R = 0x03;
			PWM0_0_CTL_R = 1;

			velocidadeAnterior = inteiro;

			Write(vetor[0]);
			Write(vetor[1]);
			Write('\n');
		}
	}
}
// FIM UART


//PWM
void InicializarPWM(void){
	SYSCTL_RCGCPWM_R |= 0x01;

	GPIO_PORTB_AFSEL_R |= (1 << 6) + (1 << 7);
	GPIO_PORTB_PCTL_R |= 0x44000000;
	GPIO_PORTB_DEN_R |= (1 << 6) + (1 << 7);

	PWM0_0_GENA_R = 0xC8;
	PWM0_0_GENB_R = 0xC8;

	PWM0_0_LOAD_R = PERIODO;
	PWM0_0_CMPA_R = (PERIODO * velocidadeAnterior) / 100;

	PWM0_0_CTL_R = 1;
	//PWM0_ENABLE_R = 0x03;
	PWM0_ENABLE_R = 0x01;
}
//FIM PWM

int main(void) {
	//LEDS
	SYSCTL_RCGCGPIO_R |= PORTF;
	GPIO_PORTF_DIR_R = 0x0E;
	GPIO_PORTF_DATA_R = 0x00;
	GPIO_PORTF_DEN_R = 0x0E;


	InicializarEncoder(); // PE2
	InicializarTimer(1000);

	InicializarUART(); // PB0 E PB1
	InicializarPWM(); // PB6 e PB7

	Write('i');
	Write('\n');

	while(1){
		__asm("WFI");
	}

	return 0;
}
