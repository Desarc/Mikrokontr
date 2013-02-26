#include "lights.h"

int main(int argc, char *argv[]) {


	initLeds();
	return 0;
}

void initLeds(void) {
	
	volatile avr32_pio_t *pioc = &AVR32_PIOC;
	pio->per = 0xff;
	pio->oer = 0xff;
	pio->sodr = 0xff;	
}