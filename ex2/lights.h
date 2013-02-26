#ifndef LIGHTS_H
#define LIGHTS_H

#include <avr32/ap7000.h>
#include <sys/interrupts.h>

int main (int argc, char *argv[]);
static void initLeds(void);

int main(void) {


	initLeds();
	return 0;
}

void initLeds(void) {
	
	volatile avr32_pio_t *pioc = &AVR32_PIOC;
	pio->per = 0xff;
	pio->oer = 0xff;
	pio->sodr = 0xff;	
}