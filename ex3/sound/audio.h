#ifndef AUDIO_H
#define AUDIO_H

/* Frequency levels for various tones */
#define SIL 0.1f
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

/* Triangle frequencies */
#define C_s 523.26f
#define D_s 587.34f
#define E_s 659.27f
#define F_s 698.47f
#define G_s 784.00f

/* Tone sample arrays */
#define default_sample_size 100
volatile int A4_wave[default_sample_size];
volatile int C5_wave[default_sample_size];
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
volatile int C_triangle[default_sample_size];
volatile int D_triangle[default_sample_size];
volatile int E_triangle[default_sample_size];
volatile int F_triangle[default_sample_size];
volatile int G_triangle[default_sample_size];
volatile int silence_wave[1] = {0};

/* Base amplitude */
const int A = 2000;

/* Frequency sampling */
#define Fs 8000f		// = 12M/256
//#define Fs 23437.5f		// = (12M/2)/256
//#define Fs 11718.75f		// = (12M/4)/256

/* Tone lengths (seconds) */
#define S 0.02			// 1/50
#define QQ 0.0625		// 1/16
#define E 0.125			// 1/8
#define Q 0.25			// 1/4
#define H 0.5			// 1/2
#define F 1
#define L 5
#define VL 10

/* Tone wave pointers */
volatile int *tone_wave_pointers[] = { A4_wave, C5_wave, G5_wave, A5_wave, B5_wave, C6_wave, D6_wave, E6_wave, F6_wave, G6_wave, A6_wave, B6_wave, C7_wave };
volatile int *triangle_wave_pointers[] = { C_triangle, D_triangle, E_triangle, F_triangle, G_triangle };

/* Sound vectors */
#define SCALE 0
float scale[] = { A4, C5, G5, A5, B5, C6, D6, E6, F6, G6, A6, B6, C7 };
float scale_tone_length[] = { S, S, S, S, S, S, S, S, S, S, S, S, S };
int scale_length = sizeof(scale)/sizeof(*(scale));

#define TRIANGLE 1
float triangle_scale[] = { C_s, D_s, E_s, F_s, G_s };
float triangle_scale_tone_length[] = { H, H, H, H, H };
int triangle_scale_length = sizeof(triangle_scale)/sizeof(*(triangle_scale));

#define SIRENE 2
float sirene[] = { C6, D6, E6, F6, E6, D6, C6, D6, E6, F6, E6, D6, C6, D6, E6, F6, E6, D6, C6 };
float sirene_tone_length[] = { QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ };
int sirene_length = sizeof(sirene)/sizeof(*(sirene));

#define TEST 3
float test_sound[] = { C7, F_s, B6, F_s, A6, F_s, G6, F_s, F6, F_s, E6, F_s, D6, F_s, C6, F_s, SIL };
float test_sound_tone_length[] = { QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, VL };
int test_sound_length = sizeof(test_sound)/sizeof(*(test_sound));

#define TETRIS 4
float tetris[] = { E6, B5, C6, D6, C6, B5, A5, A5, C6, E6, D6, C6, B5, B5, C6, D6, E6, C6, A5, A5,/* */ D6, D6, F6, A6, G6, F6, E6, E6, C6, E6, D6, C6, B5, B5, C6, D6, E6, C6, A5, A5 };
float tetris_tone_length[] = { Q, E, E, Q, E, E, Q, E, E, Q, E, E, Q, E, E, Q, Q, Q, Q, H,/* */ Q, E, E, Q, E, E, Q, E, E, Q, E, E, Q, E, E, Q, Q, Q, Q, Q };
int tetris_length = sizeof(tetris)/sizeof(*(tetris));

#define SILENCE 5
float silence[1] = { SIL };
float silence_length[1] = { F };

#endif
