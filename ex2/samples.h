
#ifndef SAMPLES_H
#define SAMPLES_H

/* frequency levels for various tones */
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

/* frequency levels as floats (for comparison) */
const float G5f = G5;	
const float A5f = A5;	
const float B5f = B5;
const float C6f = C6;
const float D6f = D6;
const float E6f = E6;
const float F6f = F6;
const float G6f = G6;
const float A6f = A6;
const float B6f = B6;
const float C7f = C7;

/* tone lengths (seconds) */

#define S 0.02			// 1/50
#define QQ 0.0625		// 1/16
#define E 0.125			// 1/8
#define Q 0.25			// 1/4
#define H 0.5			// 1/2
#define F 1

/* tone sample arrays */
#define default_sample_size 200

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
volatile int silence[1] = {0};


#define Fs 46875

#define song_length 22
#define scale_length 11
#define sirene_length 19

int *wave_pointers[scale_length] = {G5_wave, A5_wave, B5_wave, C6_wave, D6_wave, E6_wave, F6_wave, G6_wave, A6_wave, B6_wave, C7_wave };

/* sound vectors */
float song_tone[song_length] = {C6, D6, E6, F6, G6, G6, A6, A6, A6, A6, G6, F6, F6, F6, F6, E6, E6, D6, D6, D6, D6, C6 };
float song_tone_length[song_length] = {Q, Q, Q, Q, H, H, Q, Q, Q, Q, F, Q, Q, Q, Q, H, H, Q, Q, Q, Q, F };

float scale[scale_length] = {G5, A5, B5, C6, D6, E6, F6, G6, A6, B6, C7 };
float scale_tone_length[scale_length] = {S, S, S, S, S, S, S, S, S, S};

float sirene[sirene_length] = {C6, D6, E6, F6, E6, D6, C6, D6, E6, F6, E6, D6, C6, D6, E6, F6, E6, D6, C6 };
float sirene_tone_length[sirene_length] = {QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ };



#endif
