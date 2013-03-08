/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 *****************************************************************************/

#include "oeving2.h"

#define max_sample_size 500
#define default_sample_size 400
#define Fs 46875
#define song_length 22

volatile int sound_length;
const int A = 1500;
volatile int sample_size;
volatile int LED_VECTOR = 0x0;
volatile int sample_counter;
volatile int repeat_counter;
volatile int song_counter;
volatile int playing_sound = 0;


volatile int sound[max_sample_size];

volatile int C6_wave[default_sample_size];
volatile int D6_wave[default_sample_size];
volatile int E6_wave[default_sample_size];
volatile int F6_wave[default_sample_size];
volatile int G6_wave[default_sample_size];
volatile int A6_wave[default_sample_size];

volatile int *current_wave_ptr;

const float song_tone[song_length] = {C6, D6, E6, F6, G6, G6, A6, A6, A6, A6, G6, F6, F6, F6, F6, E6, E6, D6, D6, D6, D6, C6 };
const int song_tone_length[song_length] = {Q, Q, Q, Q, H, H, Q, Q, Q, Q, F, Q, Q, Q, Q, H, H, Q, Q, Q, Q, F };

int main (int argc, char *argv[]) {
	int i;
	/* pre-generating wave samples */
	for (i = 0; i < default_sample_size; i++) {
		C6_wave[i] = (int)floor(A*sin(C6*(2*M_PI)*i/Fs));
	}
	for (i = 0; i < default_sample_size; i++) {
		D6_wave[i] = (int)floor(A*sin(D6*(2*M_PI)*i/Fs));
	}
	for (i = 0; i < default_sample_size; i++) {
		E6_wave[i] = (int)floor(A*sin(E6*(2*M_PI)*i/Fs));
	}
	for (i = 0; i < default_sample_size; i++) {
		F6_wave[i] = (int)floor(A*sin(F6*(2*M_PI)*i/Fs));
	}
	for (i = 0; i < default_sample_size; i++) {
		G6_wave[i] = (int)floor(A*sin(G6*(2*M_PI)*i/Fs));
	}
	for (i = 0; i < default_sample_size; i++) {
		A6_wave[i] = (int)floor(A*sin(A6*(2*M_PI)*i/Fs));
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
	int buttons = pioc->pdsr;
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
		/* initialize song_counter, keeps track of which tone to play. get tone and length from array */
		song_counter = 0;
		f = song_tone[song_counter];
		sound_length = song_tone_length[song_counter];
		//song_counter++;
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
	//set_tone(f);				//point to the correct wave
	generate_tone(f);	
	init_sound();				//start playing the sound
}

void generate_tone(float f) {
	//sample_size = ceil(Fs/f);
	sample_size = default_sample_size;
	if (sample_size > max_sample_size) {
		sample_size = max_sample_size;
	}
	int i;
	for (i = 0; i < sample_size; i++) {
		sound[i] = (int)floor(A*sin(f*(2*M_PI)*i/Fs));
	}
}

void init_sound(void) {
	/* initialize counters */
	sample_counter = 0;
	repeat_counter = 0;
	playing_sound = 1;
	set_tone(song_tone[song_counter]);
	dac->sdr = *current_wave_ptr;
	current_wave_ptr++;
	sample_counter++;
}

void set_tone(float tone) {
	/* make current_wave_ptr point to the first sample (e.g. C6_wave[0]) of the desired wave */
	float C6f = C6;
	float D6f = D6;
	float E6f = E6;
	float F6f = F6;
	float G6f = G6;
	float A6f = A6;
	if (tone == C6f) {
		current_wave_ptr = C6_wave;
	}
	else if (tone == D6f) {
		current_wave_ptr = D6_wave;
	}
	else if (tone == E6f) {
		current_wave_ptr = E6_wave;
	}
	else if (tone == F6f) {
		current_wave_ptr = F6_wave;	
	}
	else if (tone == G6f) {
		current_wave_ptr = G6_wave;
	}
	else if (tone == A6f) {
		current_wave_ptr = A6_wave;
	}
	else {
		current_wave_ptr = sound;
	}
	sample_size = default_sample_size;
	return;
}

void abdac_isr(void) {
	/* check if the interrupts comes when we actually want to play a sound, to avoid crash */
	if (playing_sound == 0) {
		return;
	}
	/* check if tone has completed its duration */
	if (repeat_counter == sound_length) {
		/* if last tone in song, stop playing */
		if (song_counter >= song_length-1) {
			playing_sound = 0;
			dac->sdr = 0;
			return;
		}
		/* reset counters and get next tone + duration */
		sample_counter = 0;
		repeat_counter = 0;
		song_counter++;
		set_tone(song_tone[song_counter]);
		sound_length = song_tone_length[song_counter];
	}
	/* repeat sample if reached sample end */
	if (sample_counter == sample_size) {
		sample_counter = 0;
		repeat_counter++;
		set_tone(song_tone[song_counter]);
	}
	dac->sdr = *current_wave_ptr;
	sample_counter++;
	current_wave_ptr++;
	return;
}

void debounce(void) {
	int n = 0x0;
	while (n < 0xffff) {
		n++;
	}
	return;
}

