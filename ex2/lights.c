#include "lights.h"

volatile int LED_VECTOR = 0x80;
const int LED_MAX = 0x80;
const int LED_MIN = 0x01;
const int SET_ALL = 0xff;
const int button1 = 0x3fcfffff;
const int button2 = 0x7fceffff;

int main(int argc, char *argv[]) {
	initLeds();
	initButtons();
	while (1) {	
		int buttons = piob->pdsr;
		if (buttons == button1) {
			goLeft();
		}
		if (buttons == button2) {
			goRight();
		}
		
	}
	return 0;
}

void initLeds(void) {
	pioc->per = SET_ALL;
	pioc->oer = SET_ALL;
	pioc->codr = SET_ALL;
	pioc->sodr = LED_VECTOR;
}

void initButtons(void) {
	piob->per = SET_ALL;
	piob->puer = SET_ALL;
}

void goLeft(void) {
	pioc->codr = SET_ALL;
	if (LED_VECTOR == LED_MAX) {
		LED_VECTOR = LED_MIN;
	}
	else {	
		LED_VECTOR = LED_VECTOR << 1;
	}
	pioc->sodr = LED_VECTOR;
	debounce();
}

void goRight(void) {
	pioc->codr = SET_ALL;
	if (LED_VECTOR == LED_MIN) {
		LED_VECTOR = LED_MAX;
	}
	else {	
		LED_VECTOR = LED_VECTOR >> 1;
	}
	pioc->sodr = LED_VECTOR;
	debounce();
}

void debounce(void) {
	int n = 0x0;
	while (n < 0x4ffff) {
		n++;
	}
}
