
#ifndef SAMPLES_H
#define SAMPLES_H

/* frequency levels for various tones */
#define s 0.1f
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

/* triangle frequencies */
#define C_s 523.26f
#define D_s 587.34f
#define E_s 659.27f
#define F_s 698.47f
#define G_s 784.00f

/* tone sample arrays */
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


//#define Fs 46875.0f		// = 12M/256
#define Fs 23437.5f		// = (12M/2)/256
//#define Fs 11718.75f		// = (12M/4)/256

/* tone lengths (seconds) */
#define S 0.02			// 1/50
#define QQ 0.0625		// 1/16
#define E 0.125			// 1/8
#define Q 0.25			// 1/4
#define H 0.5			// 1/2
#define F 1
#define L 5
#define VL 10

#define scale_length 13
#define sirene_length 19
#define triangle_scale_length 5
#define test_sound_length 16
#define tetris_length 40

int *tone_wave_pointers[scale_length] = { A4_wave, C5_wave, G5_wave, A5_wave, B5_wave, C6_wave, D6_wave, E6_wave, F6_wave, G6_wave, A6_wave, B6_wave, C7_wave };
int *triangle_wave_pointers[triangle_scale_length] = { C_triangle, D_triangle, E_triangle, F_triangle, G_triangle };

/* sound vectors */
float scale[scale_length] = { A4, C5, G5, A5, B5, C6, D6, E6, F6, G6, A6, B6, C7 };
float scale_tone_length[scale_length] = { S, S, S, S, S, S, S, S, S, S, S, S, S };

float triangle_scale[triangle_scale_length] = { C_s, D_s, E_s, F_s, G_s };
float triangle_scale_tone_length[triangle_scale_length] = { H, H, H, H, H };

float sirene[sirene_length] = { C6, D6, E6, F6, E6, D6, C6, D6, E6, F6, E6, D6, C6, D6, E6, F6, E6, D6, C6 };
float sirene_tone_length[sirene_length] = { QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ };

float test_sound[test_sound_length] = { C7, F_s, B6, F_s, A6, F_s, G6, F_s, F6, F_s, E6, F_s, D6, F_s, C6, F_s };
float test_sound_tone_length[test_sound_length] = { QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ };

float tetris[tetris_length] = { E6, B5, C6, D6, C6, B5, A5, A5, C6, E6, D6, C6, B5, B5, C6, D6, E6, C6, A5, A5,/* */ D6, D6, F6, A6, G6, F6, E6, E6, C6, E6, D6, C6, B5, B5, C6, D6, E6, C6, A5, A5 };
float tetris_tone_length[tetris_length] = { Q, E, E, Q, E, E, Q, E, E, Q, E, E, Q, E, E, Q, Q, Q, Q, H,/* */ Q, E, E, Q, E, E, Q, E, E, Q, E, E, Q, E, E, Q, Q, Q, Q, Q };

float silence[1] = { s };
float silence_length[1] = { F };

#endif
