#include <stdio.h>

#define A4 440.00
#define B4 493.88
#define C5 523.25
#define D5 587.33
#define E5 659.26
#define F5 698.46
#define G5 783.99
#define A5 880.00
#define B5 987.77
#define C6 1046.50
#define D6 1174.66
#define E6 1318.51
#define F6 1396.91
#define G6 1567.98
#define A6 1760.00
#define B6 1975.53
#define C7 2093.00
#define D7 2349.32
#define E7 2637.02
#define F7 2793.83
#define G7 3135.96
#define A7 3520.00

#define song_length 22
#define default_sample_size 400

static void set_tone(float tone);
int main (int argc, char *argv[]);

volatile int C6_wave[default_sample_size];
volatile int D6_wave[default_sample_size];
volatile int E6_wave[default_sample_size];
volatile int F6_wave[default_sample_size];
volatile int G6_wave[default_sample_size];
volatile int A6_wave[default_sample_size];

volatile int *current_wave_ptr;

const float song_tone[song_length] = {C6, D6, E6, F6, G6, G6, A6, A6, A6, A6, G6, F6, F6, F6, F6, E6, E6, D6, D6, D6, D6, C6 };

int main (int argc, char *argv[]) {
	
	int song_counter = 0;
	song_counter++;
	printf("D6 = %3.2f\n", song_tone[song_counter]);
	float test = song_tone[song_counter];
	printf("%3.2f\n", test);
	float test2 = D6;
	printf("%3.2f\n", D6);
	if (test == test2) {
		printf("True!\n");
	}
	set_tone(song_tone[song_counter]);
	printf("C6 = %3.2f\n", song_tone[0]);
	if (song_tone[0] == C6) {
		printf("True!\n");
	}
	return 0;
}

void set_tone(float tone) {
	/* make current_wave_ptr point to the first sample (e.g. C6_wave[0]) of the desired wave */
	if (tone == C6) {
		current_wave_ptr = C6_wave;
		printf("set tone to C6!");
	}
	else if (tone == D6) {
		current_wave_ptr = D6_wave;
		printf("set tone to D6!");
	}
	else if (tone == E6) {
		current_wave_ptr = E6_wave;
	}
	else if (tone == F6) {
		current_wave_ptr = F6_wave;	
	}
	else if (tone == G6) {
		current_wave_ptr = G6_wave;
	}
	else if (tone == A6) {
		current_wave_ptr = A6_wave;
	}
	return;
}
