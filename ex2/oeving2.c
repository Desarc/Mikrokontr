/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 *****************************************************************************/

#include "oeving2.h"

volatile avr32_pio_t *piob = &AVR32_PIOB;
volatile avr32_pio_t *pioc = &AVR32_PIOC;

volatile int LED_VECTOR = 0x80;
volatile int LED_TEST = 0x08;
const int LED_MAX = 0x80;
const int LED_MIN = 0x01;
const int SET_ALL = 0xff;
const int button1 = 0x80;
const int button2 = 0x40;


int main (int argc, char *argv[]) {
  initHardware();

  while(1);
  return 0;
}

/* funksjon for å initialisere maskinvaren, må utvides */
void initHardware (void) {
 	initIntc();
	initLeds();
 	initButtons();
 	initAudio();
	return;
}

void initIntc(void) {
	set_interrupts_base((void *)AVR32_INTC_ADDRESS);
	return;
  
}

void initButtons(void) {
	pioc->per = SET_ALL;
	pioc->puer = SET_ALL;
	pioc->ier = SET_ALL;
  	register_interrupt(button_isr, AVR32_PIOC_IRQ/32, AVR32_PIOC_IRQ % 32, BUTTONS_INT_LEVEL);
	pioc->isr;	
	init_interrupts();
	return;
	
  /* (...) */
}

void clearLeds(void) {
	piob->codr = SET_ALL;
}

void initLeds(void) {
  	piob->per = SET_ALL;
	piob->oer = SET_ALL;
	clearLeds();
	piob->sodr = LED_VECTOR;
	return;
}

void initAudio(void) {
	register_interrupt( abdac_isr, AVR32_ABDAC_IRQ/32, AVR32_ABDAC_IRQ%32, ABDAC_INT_LEVEL);
	return;
  /* (...) */
}

void button_isr(void) {
	pioc->isr;
	clearLeds();
	int buttons = pioc->pdsr;
	if (buttons == button1) {
		goLeft();
	}
	if (buttons == button2) {
		goRight();
	}
	piob->sodr = LED_TEST;
	return;
}

void abdac_isr(void) {}

void goLeft(void) {
	clearLeds();
	if (LED_VECTOR == LED_MAX) {
		LED_VECTOR = LED_MIN;
	}
	else {	
		LED_VECTOR = LED_VECTOR << 1;
	}
	piob->sodr = LED_VECTOR;
	debounce();
	return;
}

void goRight(void) {
	clearLeds();
	if (LED_VECTOR == LED_MIN) {
		LED_VECTOR = LED_MAX;
	}
	else {	
		LED_VECTOR = LED_VECTOR >> 1;
	}
	piob->sodr = LED_VECTOR;
	debounce();
	return;
}

void debounce(void) {
	int n = 0x0;
	while (n < 0x1ffff) {
		n++;
	}
	return;
}
