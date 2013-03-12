#ifndef SAMPLES_H
#define SAMPLES_H

#define Fs 46875 // Frequency sample
#define A 1000 // Amptiude

/* Frequency levels for various tones */
#define A4 440.00f
#define B4 493.88f
#define C5 523.25f
#define D5 587.33f
#define E5 659.26f
#define F5 698.46f
#define G5 783.99f
#define A5 880.00f
#define B5 987.77f
#define C6 1046.50f
#define D6 1174.66f
#define E6 1318.51f
#define F6 1396.91f
#define G6 1567.98f
#define A6 1760.00f
#define B6 1975.53f
#define C7 2093.00f
#define D7 2349.32f
#define E7 2637.02f
#define F7 2793.83f
#define G7 3135.96f
#define A7 3520.00f

/* Tone lengths (seconds) */
#define S 0.02			// 1/50
#define QQ 0.0625		// 1/16
#define E 0.125			// 1/8
#define Q 0.25			// 1/4
#define H 0.5			// 1/2
#define F 1			// 1

/* Tone sample arrays */
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

/* Sounds */
#define song_length 22
#define scale_length 11
#define sirene_length 19

int *wave_pointers[scale_length] = {G5_wave, A5_wave, B5_wave, C6_wave, D6_wave, E6_wave, F6_wave, G6_wave, A6_wave, B6_wave, C7_wave };

/* Sound vectors */
float song_tone[song_length] = {C6, D6, E6, F6, G6, G6, A6, A6, A6, A6, G6, F6, F6, F6, F6, E6, E6, D6, D6, D6, D6, C6 };
float song_tone_length[song_length] = {Q, Q, Q, Q, H, H, Q, Q, Q, Q, F, Q, Q, Q, Q, H, H, Q, Q, Q, Q, F };

float scale[scale_length] = {G5, A5, B5, C6, D6, E6, F6, G6, A6, B6, C7 };
float scale_tone_length[scale_length] = {S, S, S, S, S, S, S, S, S, S};

float sirene[sirene_length] = {C6, D6, E6, F6, E6, D6, C6, D6, E6, F6, E6, D6, C6, D6, E6, F6, E6, D6, C6 };
float sirene_tone_length[sirene_length] = {QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ };

struct sound {
        float key;
        float dur;
};

struct sound jump[] = {{G5,S},{A5,S},{B5,S},{C6,S},{D6,S},{E6,S},{F6,S},{G6,S},{A6,S},{B6,S},{C7,S}};
struct sound siren[] = {{C6,QQ},{D6,QQ},{E6,QQ},{F6,QQ},{E6,QQ},{D6,QQ},{C6,QQ},{D6,QQ},{E6,QQ},{F6,QQ},{E6,QQ},{D6,QQ},{C6,QQ},{D6,QQ},{E6,QQ},{F6,QQ},{E6,QQ},{D6,QQ},{C6,QQ}};
struct sound lisa[] = {{C6,Q},{D6,Q},{E6,Q},{F6,Q},{G6,H},{G6,H},{A6,Q},{A6,Q},{A6,Q},{A6,Q},{G6,F},{F6,Q},{F6,Q},{F6,Q},{F6,Q},{E6,H},{E6,H},{D6,Q},{D6,Q},{D6,Q},{D6,Q},{C6,F}};


#endif
