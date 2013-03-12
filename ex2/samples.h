
#ifndef SAMPLES_H
#define SAMPLES_H

/* frequency levels for various tones */
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
#define Fs6 1479.98f
#define G6 1567.98f
#define A6 1760.00f
#define B6 1975.53f
#define C7 2093.00f
#define D7 2349.32f
#define E7 2637.02f
#define F7 2793.83f
#define G7 3135.96f
#define A7 3520.00f

/* sawtooth frequencies */
#define C_s 523.26f
#define D_s 587.34f
#define E_s 659.27f
#define F_s 698.47f
#define G_s 784.00f

/* square frequencies */
#define C_sq 523.23f
#define D_sq 587.31f
#define E_sq 659.24f
#define F_sq 698.44f
#define G_sq 783.97f

/* triangle frequencies */
#define C_t 523.24f
#define D_t 587.32f
#define E_t 659.25f
#define F_t 698.45f
#define G_t 783.98f

/* fm frequencies */
#define fc_1 220.0f
#define fI_1 5.0f
#define fm_1 330.0f
#define fI_2 10.0f
#define fm_2 441.0f
#define fI_3 15.0f
#define fm_3 555.0f

#define f_1 { fc_1, fm_1, fI_1 }
#define f_2 { fc_1, fm_2, fI_2 }
#define f_3 { fc_1, fm_3, fI_3 }


/* some chords/harmonics */
#define C_c { C6, E6, G6 }
#define D_c { D6, Fs6, A6 }
#define Em_c { E6, G6, B6 }
#define C_harm { C5, C6, C7 }
#define A_harm { A4, A5, A6 }

#define C 425.21f
#define D 477.29f
#define Em 525.66f
#define C_h 299.00f
#define A_h 251.43f

/* tone sample arrays */
#define default_sample_size 200

volatile int A4_wave[default_sample_size];
volatile int C5_wave[default_sample_size];
volatile int G5_wave[default_sample_size];
volatile int A5_wave[default_sample_size];
volatile int B5_wave[default_sample_size];
volatile int C6_wave[default_sample_size];
volatile int D6_wave[default_sample_size];
volatile int E6_wave[default_sample_size];
volatile int F6_wave[default_sample_size];
volatile int Fs6_wave[default_sample_size];
volatile int G6_wave[default_sample_size];
volatile int A6_wave[default_sample_size];
volatile int B6_wave[default_sample_size];
volatile int C7_wave[default_sample_size];
volatile int C_chord[default_sample_size];
volatile int D_chord[default_sample_size];
volatile int Em_chord[default_sample_size];
//volatile int C_harmonic[default_sample_size];
//volatile int A_harmonic[default_sample_size];
volatile int C_sawtooth[default_sample_size];
volatile int D_sawtooth[default_sample_size];
volatile int E_sawtooth[default_sample_size];
volatile int F_sawtooth[default_sample_size];
volatile int G_sawtooth[default_sample_size];
volatile int C_triangle[default_sample_size];
volatile int D_triangle[default_sample_size];
volatile int E_triangle[default_sample_size];
volatile int F_triangle[default_sample_size];
volatile int G_triangle[default_sample_size];
volatile int C_square[default_sample_size];
volatile int D_square[default_sample_size];
volatile int E_square[default_sample_size];
volatile int F_square[default_sample_size];
volatile int G_square[default_sample_size];
volatile int fm1_wave[default_sample_size];
volatile int fm2_wave[default_sample_size];
volatile int fm3_wave[default_sample_size];
volatile int silence[1] = {0};


#define Fs 46875.0

/* tone lengths (seconds) */
#define S 0.02			// 1/50
#define QQ 0.0625		// 1/16
#define E 0.125			// 1/8
#define Q 0.25			// 1/4
#define H 0.5			// 1/2
#define F 1
#define L 5
#define VL 10

#define song_length 22
#define scale_length 14
#define chords_scale_length 3
#define sirene_length 19
#define sawtooth_scale_length 5
#define square_scale_length 5
#define triangle_scale_length 5
#define fm_scale_length 3
#define chords_length 8
#define harmonics_length 4
#define test_sound_length 12

int *tone_wave_pointers[scale_length] = { A4_wave, C5_wave, G5_wave, A5_wave, B5_wave, C6_wave, D6_wave, E6_wave, F6_wave, Fs6_wave, G6_wave, A6_wave, B6_wave, C7_wave };
int *chords_wave_pointers[chords_scale_length] = { C_chord, D_chord, Em_chord };
int *sawtooth_wave_pointers[sawtooth_scale_length] = { C_sawtooth, D_sawtooth, E_sawtooth, F_sawtooth, G_sawtooth };
int *square_wave_pointers[square_scale_length] = { C_square, D_square, E_square, F_square, G_square };
int *triangle_wave_pointers[triangle_scale_length] = { C_triangle, D_triangle, E_triangle, F_triangle, G_triangle };
int *fm_wave_pointers[fm_scale_length] = { fm1_wave, fm2_wave, fm3_wave };

/* sound vectors */
float song_tone[song_length] = { C6, D6, E6, F6, G6, G6, A6, A6, A6, A6, G6, F6, F6, F6, F6, E6, E6, D6, D6, D6, D6, C6 };
float song_tone_length[song_length] = { Q, Q, Q, Q, H, H, Q, Q, Q, Q, F, Q, Q, Q, Q, H, H, Q, Q, Q, Q, F };

float scale[scale_length] = { A4, C5, G5, A5, B5, C6, D6, E6, F6, Fs6, G6, A6, B6, C7 };
float scale_tone_length[scale_length] = { S, S, S, S, S, S, S, S, S, S, S, S, S, S };

float sawtooth_scale[sawtooth_scale_length] = { C_s, D_s, E_s, F_s, G_s };
float sawtooth_scale_tone_length[sawtooth_scale_length] = { H, H, H, H, H };

float square_scale[square_scale_length] = { C_sq, D_sq, E_sq, F_sq, G_sq };
float square_scale_tone_length[square_scale_length] = { H, H, H, H, H };

float triangle_scale[triangle_scale_length] = { C_t, D_t, E_t, F_t, G_t };
float triangle_scale_tone_length[triangle_scale_length] = { H, H, H, H, H };

float chords_scale[chords_scale_length][3] = { C_c, D_c, Em_c };

float fm_scale[fm_scale_length][3] = { f_1, f_2, f_3 };

float sirene[sirene_length] = { C6, D6, E6, F6, E6, D6, C6, D6, E6, F6, E6, D6, C6, D6, E6, F6, E6, D6, C6 };
float sirene_tone_length[sirene_length] = { QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ };

float test_sound[test_sound_length] = { F6, E_s, E6, D6, C_sq, D_sq, C, C7, B6, A6, G6, F_s };
float test_sound_tone_length[test_sound_length] = { Q, H, Q, Q, H, Q, E, QQ, QQ, QQ, QQ, H };

float chords[chords_length] = { Em, C, D, Em, Em, C, D, Em };
float chords_tone_length[chords_length] = { H, H, H, H, H, H, H, H };

float fm_sound[fm_scale_length] = { fm_1, fm_2, fm_3 };
float fm_sound_tone_length[fm_scale_length] = { F, F, F };

#endif
