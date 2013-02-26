#include "lights.h"

int main(int argc, char *argv[]) {
	initLeds();
	initButtons();

	int none = 0x00;
	while (1) {	
		int buttons = piob->psdr;
		if (buttons != none)
			pioc->sodr = 0xff;
		else
			pioc->codr = 0xff;
	}
	return 0;
}

void initLeds(void) {
	pioc->per = 0xff;
	pioc->oer = 0xff;
	pioc->codr = 0xff;
	//pioc->sodr = 0x80;
}

void initButtons(void) {
	piob->per = 0xff;
	piob->puer = 0xff;
}

