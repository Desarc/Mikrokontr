/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 *****************************************************************************/

#include "oeving2.h"
#include "samples.h"

const int A = 1500;
volatile int LED_VECTOR = 0x0;
volatile int sample_size;
volatile int sample_counter;
volatile int repeat_counter;
volatile int sound_counter;
volatile int tone_length;
volatile int sound_length;
volatile int playing_sound = 0;

volatile int *current_wave_ptr;
volatile float *current_sound_ptr;
volatile float *current_sound_tl_ptr;

int main (int argc, char *argv[]) {
	int i;
	/* pre-generating tone sample vectors */
	for (i = 0; i < scale_length; i++) {
		current_wave_ptr = wave_pointers[i];
		generate_tone(scale[i]);
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

/* generate the sample vector for ~one period of a pure sine */
void generate_tone(float f) {
	set_sample_size(f);
	int j;
	for (j = 0; j < sample_size; j++) {
		*current_wave_ptr = (int)floor(A*sin(f*(2*M_PI)*j/Fs));
		current_wave_ptr++;
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
		current_sound_ptr = scale;
		current_sound_tl_ptr = scale_tone_length;
		sound_length = scale_length;
		LED_VECTOR = LED6;
	}
	else if (buttons == BUTTON5) {
		current_sound_ptr = sirene;
		current_sound_tl_ptr = sirene_tone_length;
		sound_length = sirene_length;
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

/* initialize counters and pointers */
void init_sound(void) {
	sound_counter = 0;
	sample_counter = 0;
	repeat_counter = 0;
	playing_sound = 1;
	float tone = *current_sound_ptr;
	set_tone(tone);
	float length = *current_sound_tl_ptr;
	set_tone_length(tone, length);
	dac->sdr = *current_wave_ptr;
	current_wave_ptr++;
	sample_counter++;
}

/* set the size of the sample vector, depending on the frequency of the tone */
void set_sample_size(float tone) {
	sample_size = (int)ceil(Fs/tone);
	if (sample_size > default_sample_size) {
		sample_size = default_sample_size;
	}
}

/* make current_wave_ptr point to the first sample (e.g. C6_wave[0]) of the desired tone */
void set_tone(float tone) {
	set_sample_size(tone);
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
		current_wave_ptr = silence;
		sample_size = 1;
	}
	return;
}

/* set number of sample vector repeats according to frequency and desired length */
void set_tone_length(float tone, float length) {
	tone_length = (int)(tone * length);
}

void abdac_isr(void) {
	/* check if the interrupts comes when we actually want to play a sound, to avoid crash */
	if (playing_sound == 0) {
		return;
	}
	/* check if tone has completed its duration */
	if (repeat_counter >= tone_length) {
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
		float tone = *current_sound_ptr;
		set_tone(tone);
		float length = *current_sound_tl_ptr;
		set_tone_length(tone, length);
	}
	/* repeat sample vector if last sample played */
	if (sample_counter >= sample_size) {
		sample_counter = 0;
		repeat_counter++;
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

