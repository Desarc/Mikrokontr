/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 *****************************************************************************/

#include "oeving2.h"

volatile int LED_VECTOR = 0x0;

int main (int argc, char *argv[]) {
  initHardware();
  while(1);
  return 0;
}


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
}

void clearLeds(void) {
	piob->codr = SET_ALL;
	return;
}

void setLeds(void) {
	piob->sodr = LED_VECTOR;
	return;
}

void initLeds(void) {
  	piob->per = SET_ALL;
	piob->oer = SET_ALL;
	clearLeds();
	return;
}

void initAudio(void) {
	register_interrupt( abdac_isr, AVR32_ABDAC_IRQ/32, AVR32_ABDAC_IRQ%32, ABDAC_INT_LEVEL);
	sm->gcctrl[6] = 0x04;
	piob->PDR.p20 = 1;
	piob->PDR.p21 = 1;
	piob->ASR.p20 = 1;
	piob->ASR.p21 = 1;
	dac->CR.en = 1;
	dac->IER.tx_ready = 1;
	
	return;
}

void button_isr(void) {
	pioc->isr;
	clearLeds();
	int buttons = pioc->pdsr;
	if (buttons == BUTTON7) {
		dac->sdr = rand() % 22000;
		LED_VECTOR = LED7;
	}
	else if (buttons == BUTTON6) {
		dac->sdr = 0;
		LED_VECTOR = LED6;
	}
	else if (buttons == BUTTON5) {
		LED_VECTOR = LED5;
	}
	else if (buttons == BUTTON4) {
		LED_VECTOR = LED4;
	}
	else if (buttons == BUTTON3) {
		LED_VECTOR = LED3;
	}
	else if (buttons == BUTTON2) {
		LED_VECTOR = LED2;
	}
	else if (buttons == BUTTON1) {
		LED_VECTOR = LED1;
	}
	else if (buttons == BUTTON0) {
		LED_VECTOR = LED0;
	}
	setLeds();
	debounce();
	return;
}

void abdac_isr(void) {

	return;
}

void debounce(void) {
	int n = 0x0;
	while (n < 0x01ffff) {
		n++;
	}
	return;
}

