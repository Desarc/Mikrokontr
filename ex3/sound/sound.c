#include "sound.h"
#include "linux/soundcard.h"
#include "sys/mman.h"
#include "stdio.h"
#include "audio.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "unistd.h"
#include "math.h"

/* Size of the current sample vector */
volatile int sample_size;
/* Counter to keep track of how many samples of the current vector have been played */
volatile int sample_counter;
/* Length of current tone (number of times the sample vector should be repeated) */
volatile int tone_length;
/* Counter to keep track of how many times the sample vector has been repeated */
volatile int repeat_counter;
/* Number of tones in the sound */
volatile int sound_length;
/* Counter to keep track of how many tones of the sound have been played */
volatile int tone_counter;

volatile int playing_sound = 0;
volatile int prev_button = 0;

/* Pointer to the next sample value */
volatile int *current_wave_ptr;
/* Pointer to the frequency value of the next tone */
volatile float *current_sound_ptr;
/* Pointer to the length value (in seconds) of the next tone */
volatile float *current_sound_tl_ptr;

int main (int argc, char *argv[]) {
	pregenerateTones();
	open_driver();
	play_sound(TETRIS);
}

int open_driver(void) {
	
	int fd_dsp;
	char *dsp_p = 0;
	int sample_size = 8;

	fd_dsp = open("/dev/dsp", O_RDWR);
	
	if (fd_dsp < 0) {
		perror("open: ");
		return 1;
	}
	printf("sound device opened, %i\n", fd_dsp);
	//if (lseek(fd_dsp, 1, SEEK_SET) < 0) {
		//perror("lseek: ");
		//return 1;
	//}

	int dsp_rate = 8000;
	int dsprate_success = -1;
	dsprate_success = ioctl(fd_dsp, SOUND_PCM_WRITE_RATE, &dsp_rate);
	printf("success setting dsp rate? %i\n", dsprate_success);
	
	int sample_size_success = -1;
	sample_size_success = ioctl(fd_dsp, SOUND_PCM_WRITE_BITS, &sample_size);
	printf("set sample size success?: %i\n", sample_size_success);
	
	while(1) {
		int sample = rand()%0xff;
		printf("%i\n", sample);
		int write_success = write(fd_dsp, &sample, 1);
		printf("number of bytes written: %i\n", write_success);
		if (write_success < 0) {
		    perror("write: ");
		    return 1;
		}
	}

	/*dsp_p = (char *)mmap(NULL, sample_size, PROT_WRITE, MAP_SHARED, fd_dsp, 0);
	
	if ((int)dsp_p == -1) {
    	    perror("Error: failed to map sound device to memory");
        	exit(4);
    	}
	printf("The sound device was mapped to memory successfully.\n");*/
	

	
	close(fd_dsp);
}

/* Pre-generate sine and triangle tones */
void pregenerateTones(void) {
	int i;
	/* Pre-generating sine sample vectors */
	for (i = 0; i < scale_length; i++) {
		current_wave_ptr = tone_wave_pointers[i];
		generateSine(scale[i]);
	}
	/* Pre-generating triangle sample vectors */
	for (i = 0; i < triangle_scale_length; i++) {
		current_wave_ptr = triangle_wave_pointers[i];
		generateTriangle(triangle_scale[i]);
	}
}

/* Generate the sample vector for ~one period of a pure sine wave */
void generateSine(float f) {
	set_sample_size(f);
	int i;
	for (i = 0; i < sample_size; i++) {
		*current_wave_ptr = (int) floor(A*sin(f*(2*M_PI)*i/Fs));
		current_wave_ptr++;
	}
}

/* Generate the sample vector for ~one period of a triangle wave */
void generateTriangle(float f) {
	set_sample_size(f);
	int i;
	int k;
	for (i = 0; i < sample_size; i++) {
		float sample = 0;
		for (k = 0; k < 10; k++) {
                	sample += ((A*(3.0/4.0)*8)/pow(M_PI,2.0))*((pow(-1,k)*sin((2*k+1)*2.0*M_PI*f*(i/Fs))/pow(2*k+1,2.0))+1);
		}
		*current_wave_ptr = (int) sample;
		current_wave_ptr++;
	}
}

void play_sound(int sound) {
	if (sound == TEST) {
		current_sound_ptr = test_sound;
		current_sound_tl_ptr = test_sound_tone_length;
		sound_length = test_sound_length;
	}
	else if (sound == SCALE) {
		current_sound_ptr = scale;
		current_sound_tl_ptr = scale_tone_length;
		sound_length = scale_length;
	}
	else if (sound == SIRENE) {
		current_sound_ptr = sirene;
		current_sound_tl_ptr = sirene_tone_length;
		sound_length = sirene_length;
	}
	else if (sound == TETRIS) {
		current_sound_ptr = tetris;
		current_sound_tl_ptr = tetris_tone_length;
		sound_length = tetris_length;
	}
	else if (sound == SILENCE) {
		current_sound_ptr = silence;
		current_sound_tl_ptr = silence_length;
		sound_length = 1;
	}

	init_sound(); // Start playing the sound
}

/* Set the size of the sample vector, depending on the frequency of the tone */
void set_sample_size(float tone) {
	sample_size = (int)ceil(Fs/tone);
	if (sample_size > default_sample_size) {
		sample_size = default_sample_size;
	}
}

/* Make current_wave_ptr point to the first sample (e.g. C6_wave[0]) of the desired tone sample vector */
void set_tone(float tone) {
	set_sample_size(tone);
	if (tone == G5) current_wave_ptr = G5_wave;
	else if (tone == A5) current_wave_ptr = A5_wave;
	else if (tone == B5) current_wave_ptr = B5_wave;
	else if (tone == C6) current_wave_ptr = C6_wave;
	else if (tone == D6) current_wave_ptr = D6_wave;
	else if (tone == E6) current_wave_ptr = E6_wave;
	else if (tone == F6) current_wave_ptr = F6_wave;	
	else if (tone == G6) current_wave_ptr = G6_wave;
	else if (tone == A6) current_wave_ptr = A6_wave;
	else if (tone == B6) current_wave_ptr = B6_wave;
	else if (tone == C7) current_wave_ptr = C7_wave;
	else if (tone == C_s) current_wave_ptr = C_triangle;
	else if (tone == D_s) current_wave_ptr = D_triangle;
	else if (tone == E_s) current_wave_ptr = E_triangle;
	else if (tone == F_s) current_wave_ptr = F_triangle;
	else if (tone == G_s) current_wave_ptr = G_triangle;
	/* Default: don't play any sound */
	else {
		current_wave_ptr = silence_wave;
		sample_size = 1;
	}
}

/* Initialize sound counters and pointers, and send first sample to ABDAC */
void init_sound(void) {
	tone_counter = 0;
	sample_counter = 0;
	repeat_counter = 0;
	playing_sound = 1;
	set_tone(*current_sound_ptr);
	set_tone_length(*current_sound_ptr, *current_sound_tl_ptr);
	//TODO send first sample to soundcard
	//dac->SDR.channel0 = (short) *current_wave_ptr;
	//dac->SDR.channel1 = (short) *current_wave_ptr;	
	current_wave_ptr++;
	sample_counter++;
}

/* Set number of sample vector repeats according to frequency and desired tone length */
void set_tone_length(float tone, float length) {
	tone_length = (int) round(tone*length);
}

void play_sample(void) {
	/* Check if the interrupt comes when we actually want to play a sound, to avoid crash/failure */
	if (playing_sound == 0) {
		return;
	}
	/* Check if tone has completed its duration */
	if (repeat_counter >= tone_length) {
		/* If current tone is last tone in song, stop playing */
		if (tone_counter >= sound_length) {
			playing_sound = 0;
			/*
			* write zero to soundcard //TODO
			*
			*/
			return;
		}
		/* Reset sample and repeat counters, and get next tone + duration */
		sample_counter = 0;
		repeat_counter = 0;
		tone_counter++;
		current_sound_ptr++;
		current_sound_tl_ptr++;
		set_tone(*current_sound_ptr);
		set_tone_length(*current_sound_ptr, *current_sound_tl_ptr);
	}
	/* Repeat sample vector if last sample played */
	if (sample_counter >= sample_size) {
		sample_counter = 0;
		repeat_counter++;
		set_tone(*current_sound_ptr);
	}
	/* Send sample to soundcard */ //TODO
	//dac->SDR.channel0 = (short) *current_wave_ptr;
	//dac->SDR.channel1 = (short) *current_wave_ptr;
	sample_counter++;
	current_wave_ptr++;
}
