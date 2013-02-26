#include "lights.h"

int main(int argc, char *argv[]) {


	initLeds();
	return 0;
}

void initLeds(void) {
	
	volatile avr32_pio_t *pioc = &AVR32_PIOC;
	pioc->per = 0xff;
	pioc->oer = 0xff;
	pioc->sodr = 0xf0;	
}

