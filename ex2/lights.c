#include "lights.h"

volatile int LED_VECTOR = 0x80;
const int LED_TEST = 0x08;
//volatile int buttons = 0x0;
const int LED_MAX = 0x80;
const int LED_MIN = 0x01;
const int SET_ALL = 0xff;
const int button7 = 0xffff3f7f;
const int button6 = 0xffff3fbf;
const int button5 = 0xffff3fdf;
const int button4 = 0xffff3fef;
const int button3 = 0xffff3ff7;
const int button2 = 0xffff3ffb;
const int button1 = 0xffff3ffd;
const int button0 = 0xffff3ffe;

int main(int argc, char *argv[]) {
	
	initLeds();
	initInterrupts();
	initButtons();
	while(1);
	return 0;
}

void interruptRoutine(void) {
	pioc->isr;
	clearLeds();
	int buttons = pioc->pdsr;
	if (buttons == button7) {
		goLeft();
	}
	if (buttons == button6) {
		goRight();
	}
	piob->sodr = LED_VECTOR;
	debounce();
	return;
}

void initLeds(void) {
	piob->per = SET_ALL;
	piob->oer = SET_ALL;
	clearLeds();
	piob->sodr = LED_VECTOR;
	return;
}

void initButtons(void) {
	pioc->per = SET_ALL;
	pioc->puer = SET_ALL;
	pioc->ier = SET_ALL;
  	register_interrupt(interruptRoutine, AVR32_PIOC_IRQ/32, AVR32_PIOC_IRQ % 32, BUTTONS_INT_LEVEL);
	pioc->isr;	
	init_interrupts();
	return;
}

void initInterrupts(void) {
	set_interrupts_base((void *)AVR32_INTC_ADDRESS);
	return;
}

void clearLeds(void) {
	piob->codr = SET_ALL;
}

void goLeft(void) {
	if (LED_VECTOR == LED_MAX) {
		LED_VECTOR = LED_MIN;
	}
	else {	
		LED_VECTOR = LED_VECTOR << 1;
	}
	return;
}

void goRight(void) {
	if (LED_VECTOR == LED_MIN) {
		LED_VECTOR = LED_MAX;
	}
	else {	
		LED_VECTOR = LED_VECTOR >> 1;
	}
	return;
}

void debounce(void) {
	int n = 0x0;
	while (n < 0x1ffff) {
		n++;
	}
	return;
}
