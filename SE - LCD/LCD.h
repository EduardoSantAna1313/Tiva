void inicializarLCD(void) ;

void clear(void);

void setCursor(uint8_t lin, uint8_t col);

void delay(uint32_t us);

void outCmd(unsigned char command);

void printChar(unsigned char c);

void printString(char *ponteiro);

void printInt(uint32_t num);


