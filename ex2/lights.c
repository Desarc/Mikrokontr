#include "lights.h"

volatile int LED_VECTOR = 0x80;
volatile int LED_TEST = 0x08;
const int LED_MAX = 0x80;
const int LED_MIN = 0x01;
const int SET_ALL = 0xff;
const int button1 = 0x3fcfffff;
const int button2 = 0x7fceffff;

int main(int argc, char *argv[]) {
	
	initLeds();
	initInterrupts();
	initButtons();
	
	while(1) {
		//interruptRoutine();
	}
	return 0;
}

void interruptRoutine(void) {
	pioc->isr;
	clearLeds();
	int buttons = pioc->pdsr;
	if (buttons == button1) {
		goLeft();
	}
	if (buttons == button2) {
		goRight();
	}
	goLeft();
	piob->sodr = LED_VECTOR;
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
	while (n < 0x01ffff) {
		n++;
	}
	return;
}
