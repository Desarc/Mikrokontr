/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 *****************************************************************************/

#include "oeving2.h"

#define max_sample_size 1024
#define sound_length 5000
#define Fs 46875

volatile int sample_size;
volatile int LED_VECTOR = 0x0;
volatile int sound[max_sample_size];
volatile int sound2[107];
volatile int counter;
volatile int counter2;
volatile int playingSound = 0;

int main (int argc, char *argv[]) {
	int i;
	for (i = 0; i < max_sample_size; i++) {
		sound[i] = 0;
	}
	for (i = 0; i < 107; i++) {
		sound2[i] = 0;
	}
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
	pm->gcctrl[6] = 0x5;  //using OSC1
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
		playSound(SOUND7);
		LED_VECTOR = LED7;
	}
	else if (buttons == BUTTON6) {
		playSound(SOUND6);
		LED_VECTOR = LED6;
	}
	else if (buttons == BUTTON5) {
		playSound(SOUND5);
		LED_VECTOR = LED5;
	}
	else if (buttons == BUTTON4) {
		playSound(SOUND4);
		LED_VECTOR = LED4;
	}
	else if (buttons == BUTTON3) {
		playSound(SOUND3);
		LED_VECTOR = LED3;
	}
	else if (buttons == BUTTON2) {
		playSound(SOUND2);
		LED_VECTOR = LED2;
	}
	else if (buttons == BUTTON1) {
		playSound(SOUND1);
		LED_VECTOR = LED1;
	}
	else if (buttons == BUTTON0) {
		playSound(SOUND0);
		LED_VECTOR = LED0;
	}
	setLeds();
	debounce();
	return;
}

void playSound(int code) {
	float f;
	if (code == SOUND7) {
		f = A4;
		//sample_size = 107;
	}
	else if (code == SOUND6) {
		f = A4;
		//sample_size = 108;
	}
	else if (code == SOUND5) {
		f = A4;
		//sample_size = 109;
	}
	else if (code == SOUND4) {
		f = D9;
		//sample_size = 100;
	}
	else if (code == SOUND3) {
		f = E9;
		//sample_size = 100;
	}
	else if (code == SOUND2) {
		f = F9;
		//sample_size = 100;
	}
	else if (code == SOUND1) {
		f = G9;
		//sample_size = 100;
	}
	else if (code == SOUND0) {
		f = A4;
		//sample_size = 100;
	}
	sample_size = ceil(Fs/f);
	if (sample_size > max_sample_size) {
		sample_size = max_sample_size;
	}
	
	int A = 1500;
	int i;
	for (i = 0; i < sample_size; i++) {
		sound[i] = (int)floor(A*sin(f*(2*M_PI)*i/Fs));
	}

	//for (i = 0; i < 107; i++) {
	//	sound2[i] = (int)floor(A*sin(f*(2*M_PI)*i/46875));
	//}
	counter = 0;
	counter2 = 0;
	playingSound = 1;
	dac->sdr = sound[counter];
	counter++;
}

void abdac_isr(void) {
	//piob->isr;
	if (playingSound == 0) {
		return;
	}
	if (counter2 == sound_length) {
		playingSound = 0;
		return;
	}
	if (counter == sample_size) {
		counter = 0;
		counter2++;
	}
	dac->sdr = sound[counter];
	counter++;
	return;
}

void debounce(void) {
	int n = 0x0;
	while (n < 0x1ffff) {
		n++;
	}
	return;
}

