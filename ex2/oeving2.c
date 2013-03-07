/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 *****************************************************************************/

#include "oeving2.h"

#define max_sample_size 5000
#define Fs 46875
#define song_length 22

volatile int sound_length;
volatile int A = 1500;
volatile int sample_size;
volatile int LED_VECTOR = 0x0;
volatile int sound[max_sample_size];
volatile int sample_counter;
volatile int repeat_counter;
volatile int song_counter;
volatile int playing_sound = 0;
volatile int skip = 0;


const float song[song_length] = {C6, D6, E6, F6, G6, G6, A6, A6, A6, A6, G6, F6, F6, F6, F6, E6, E6, D6, D6, D6, D6, C6};
const int song_tone_length[song_length] = {Q, Q, Q, Q, H, H, Q, Q, Q, Q, F, Q, Q, Q, Q, H, H, Q, Q, Q, Q, F};

int main (int argc, char *argv[]) {
	//int i;
	//for (i = 0; i < max_sample_size; i++) {
	//	sound[i] = 0;
	//}
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
	int buttons = pioc->pdsr;
	if (skip == 1) {
		skip = 0;
		//return;
	}
	else {
		skip = 1;
	}
	clearLeds();
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
		song_counter = 0;
		f = song[song_counter];
		sound_length = song_tone_length[song_counter];
		song_counter++;
	}
	else if (code == SOUND6) {
		f = D6;
	}
	else if (code == SOUND5) {
		f = E6;
	}
	else if (code == SOUND4) {
		f = F6;
	}
	else if (code == SOUND3) {
		f = G6;
	}
	else if (code == SOUND2) {
		f = A6;
	}
	else if (code == SOUND1) {
		f = B6;
	}
	else if (code == SOUND0) {
		f = C7;
	}
	generate_tone(f);
	init_sound();	
}

void generate_tone(float f) {
	sample_size = ceil(Fs/f);
	if (sample_size > max_sample_size) {
		sample_size = max_sample_size;
	}
	int i;
	for (i = 0; i < sample_size; i++) {
		sound[i] = (int)floor(A*sin(f*(2*M_PI)*i/Fs));
	}
}

void init_sound(void) {
	sample_counter = 0;
	repeat_counter = 0;
	playing_sound = 1;
	dac->sdr = sound[sample_counter];
	sample_counter++;
}


void abdac_isr(void) {
	if (playing_sound == 0) {
		return;
	}
	if (song_counter == song_length) {
		playing_sound = 0;
		return;
	}
	if (repeat_counter == sound_length) {
		sample_counter = 0;
		repeat_counter = 0;
		generate_tone(song[song_counter]);
		sound_length = song_tone_length[song_counter];
		song_counter++;
	}
	
	if (sample_counter == sample_size) {
		sample_counter = 0;
		repeat_counter++;
	}
	dac->sdr = sound[sample_counter];
	sample_counter++;
	return;
}

void debounce(void) {
	int n = 0x0;
	while (n < 0xffff) {
		n++;
	}
	return;
}

