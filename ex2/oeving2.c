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
#define test_sound_length 10

volatile int tone_length;
const int A = 1500;
volatile int sample_size;
volatile int LED_VECTOR = 0x0;
volatile int sample_counter;
volatile int repeat_counter;
volatile int sound_counter;
volatile int sound_length;
volatile int playing_sound = 0;


volatile int sound[max_sample_size];

volatile int G5_wave[default_sample_size];
volatile int A5_wave[default_sample_size];
volatile int B5_wave[default_sample_size];
volatile int C6_wave[default_sample_size];
volatile int D6_wave[default_sample_size];
volatile int E6_wave[default_sample_size];
volatile int F6_wave[default_sample_size];
volatile int G6_wave[default_sample_size];
volatile int A6_wave[default_sample_size];
volatile int B6_wave[default_sample_size];
volatile int C7_wave[default_sample_size];

volatile int *current_wave_ptr;
volatile float *current_sound_ptr;
volatile int *current_sound_tl_ptr;

float test_sound[test_sound_length] = {G5, A5, B5, C6, D6, F6, G6, A6, B6, C7 };
int test_sound_tone_length[test_sound_length] = {S, S, S, S, S, S, S, S, S, S};

float song_tone[song_length] = {C6, D6, E6, F6, G6, G6, A6, A6, A6, A6, G6, F6, F6, F6, F6, E6, E6, D6, D6, D6, D6, C6 };
int song_tone_length[song_length] = {Q, Q, Q, Q, H, H, Q, Q, Q, Q, F, Q, Q, Q, Q, H, H, Q, Q, Q, Q, F };

int main (int argc, char *argv[]) {
	int i;
	/* pre-generating wave samples */
	for (i = 0; i < default_sample_size; i++) {
		G5_wave[i] = (int)floor(A*sin(G5*(2*M_PI)*i/Fs));
	}
	for (i = 0; i < default_sample_size; i++) {
		A5_wave[i] = (int)floor(A*sin(A5*(2*M_PI)*i/Fs));
	}
	for (i = 0; i < default_sample_size; i++) {
		B5_wave[i] = (int)floor(A*sin(B5*(2*M_PI)*i/Fs));
	}
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
	for (i = 0; i < default_sample_size; i++) {
		B6_wave[i] = (int)floor(A*sin(B6*(2*M_PI)*i/Fs));
	}
	for (i = 0; i < default_sample_size; i++) {
		C7_wave[i] = (int)floor(A*sin(C7*(2*M_PI)*i/Fs));
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

void button_isr(void) {
	pioc->isr;
	int buttons = pioc->pdsr;
	clearLeds();
	if (buttons == BUTTON7) {
		current_sound_ptr = song_tone;
		current_sound_tl_ptr = song_tone_length;
		sound_length = song_length;
		LED_VECTOR = LED7;
	}
	else if (buttons == BUTTON6) {
		current_sound_ptr = test_sound;
		current_sound_tl_ptr = test_sound_tone_length;
		sound_length = test_sound_length;
		LED_VECTOR = LED6;
	}
	else if (buttons == BUTTON5) {
		//playSound(SOUND5);
		LED_VECTOR = LED5;
	}
	else if (buttons == BUTTON4) {
		//playSound(SOUND4);
		LED_VECTOR = LED4;
	}
	else if (buttons == BUTTON3) {
		//playSound(SOUND3);
		LED_VECTOR = LED3;
	}
	else if (buttons == BUTTON2) {
		//playSound(SOUND2);
		LED_VECTOR = LED2;
	}
	else if (buttons == BUTTON1) {
		//playSound(SOUND1);
		LED_VECTOR = LED1;
	}
	else if (buttons == BUTTON0) {
		//playSound(SOUND0);
		LED_VECTOR = LED0;
	}
	init_sound();				//start playing the sound
	setLeds();
	debounce();
	return;
}

void init_sound(void) {
	/* initialize counters */
	sound_counter = 0;
	sample_counter = 0;
	repeat_counter = 0;
	playing_sound = 1;
	//set_tone(song_tone[sound_counter]);
	set_tone(*current_sound_ptr);
	tone_length = *current_sound_tl_ptr;
	dac->sdr = *current_wave_ptr;
	current_wave_ptr++;
	sample_counter++;
}

void set_tone(float tone) {
	/* make current_wave_ptr point to the first sample (e.g. C6_wave[0]) of the desired wave */
	if (tone == G5f) {
		current_wave_ptr = G5_wave;
	}
	else if (tone == A5f) {
		current_wave_ptr = A5_wave;
	}	
	else if (tone == B5f) {
		current_wave_ptr = B5_wave;
	}	
	else if (tone == C6f) {
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
	else if (tone == B6f) {
		current_wave_ptr = B6_wave;
	}
	else if (tone == C7f) {
		current_wave_ptr = C7_wave;
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
	if (repeat_counter == tone_length) {
		/* if last tone in song, stop playing */
		if (sound_counter >= sound_length-1) {
			playing_sound = 0;
			dac->sdr = 0;
			return;
		}
		/* reset counters and get next tone + duration */
		sample_counter = 0;
		repeat_counter = 0;
		sound_counter++;
		current_sound_ptr++;
		current_sound_tl_ptr++;
		//set_tone(song_tone[sound_counter]);
		//tone_length = song_tone_length[sound_counter];
		set_tone(*current_sound_ptr);
		tone_length = *current_sound_tl_ptr;
	}
	/* repeat sample if reached sample end */
	if (sample_counter == sample_size) {
		sample_counter = 0;
		repeat_counter++;
		//set_tone(song_tone[sound_counter]);
		set_tone(*current_sound_ptr);
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

