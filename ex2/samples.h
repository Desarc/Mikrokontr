
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
#define Fs6 1479.98
#define G6 1567.98
#define A6 1760.00
#define B6 1975.53
#define C7 2093.00
#define D7 2349.32
#define E7 2637.02
#define F7 2793.83
#define G7 3135.96
#define A7 3520.00
#define saw 440

/* frequency levels as floats (for comparison) */
const float A4f = A4;
const float C5f = C5;
const float G5f = G5;	
const float A5f = A5;	
const float B5f = B5;
const float C6f = C6;
const float D6f = D6;
const float E6f = E6;
const float F6f = F6;
const float Fs6f = Fs6;
const float G6f = G6;
const float A6f = A6;
const float B6f = B6;
const float C7f = C7;


/* sawtooth frequencies */
#define C6_s 1046.51
#define D6_s 1174.67
#define E6_s 1318.52
#define F6_s 1396.92
#define G6_s 1567.99

const float C6_sf = C6;
const float D6_sf = D6;
const float E6_sf = E6;
const float F6_sf = F6;
const float G6_sf = G6;


/* some chords/harmonics */
#define C_c { C6, E6, G6 }
#define D_c { D6, Fs6, A6 }
#define Em_c { E6, G6, B6 }
#define C_harm { C5, C6, C7 }
#define A_harm { A4, A5, A6 }

#define C 425.21
#define D 477.29
#define Em 525.66
#define C_h 299.00
#define A_h 251.43

const float Cf = C;
const float Df = D;
const float Emf = Em;
const float C_hf = C_h;
const float A_hf = A_h;

/* tone lengths (seconds) */
#define S 0.02			// 1/50
#define QQ 0.0625		// 1/16
#define E 0.125			// 1/8
#define Q 0.25			// 1/4
#define H 0.5			// 1/2
#define F 1
#define L 5

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
volatile int C_harmonic[default_sample_size];
volatile int A_harmonic[default_sample_size];
volatile int C6_sawtooth[default_sample_size];
volatile int D6_sawtooth[default_sample_size];
volatile int E6_sawtooth[default_sample_size];
volatile int F6_sawtooth[default_sample_size];
volatile int G6_sawtooth[default_sample_size];
volatile int silence[1] = {0};


#define Fs 46875

#define song_length 22
#define scale_length 14
#define harmonics_scale_length 5
#define sirene_length 19
#define sawtooth_scale_length 5
#define chords_length 8
#define harmonics_length 4

int *tone_wave_pointers[scale_length] = { A4_wave, C5_wave, G5_wave, A5_wave, B5_wave, C6_wave, D6_wave, E6_wave, F6_wave, Fs6_wave, G6_wave, A6_wave, B6_wave, C7_wave };
int *harmonics_wave_pointers[harmonics_scale_length] = { C_chord, D_chord, Em_chord, C_harmonic, A_harmonic };
int *sawtooth_wave_pointers[sawtooth_scale_length] = { C6_sawtooth, D6_sawtooth, E6_sawtooth, F6_sawtooth, G6_sawtooth };

/* sound vectors */
float song_tone[song_length] = { C6, D6, E6, F6, G6, G6, A6, A6, A6, A6, G6, F6, F6, F6, F6, E6, E6, D6, D6, D6, D6, C6 };
float song_tone_length[song_length] = { Q, Q, Q, Q, H, H, Q, Q, Q, Q, F, Q, Q, Q, Q, H, H, Q, Q, Q, Q, F };

float scale[scale_length] = { A4, C5, G5, A5, B5, C6, D6, E6, F6, Fs6, G6, A6, B6, C7 };
float scale_tone_length[scale_length] = { S, S, S, S, S, S, S, S, S, S, S, S, S, S };

float sawtooth_scale[sawtooth_scale_length] = { C6_s, D6_s, E6_s, F6_s, G6_s };
float sawtooth_scale_tone_length[sawtooth_scale_length] = { H, H, H, H, H };

float harmonics_scale[harmonics_scale_length][3] = { C_c, D_c, Em_c, C_harm, A_harm };
float harmonics_scale_tone_length[harmonics_scale_length] = { H, H, H, H, H };

float sirene[sirene_length] = { C6, D6, E6, F6, E6, D6, C6, D6, E6, F6, E6, D6, C6, D6, E6, F6, E6, D6, C6 };
float sirene_tone_length[sirene_length] = { QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ, QQ };

float chords[chords_length] = { Em, C, D, Em, Em, C, D, Em };
float chords_tone_length[chords_length] = { H, H, H, H, H, H, H, H };

float chords_comp[chords_length] = { E6, C6, D6, E6, E6, C6, D6, E6 };
float chords_comp_tone_length[chords_length] = { H, H, H, H, H, H, H, H };

float harmonics[harmonics_length] = { C_h, A_h, C_h, A_h };
float harmonics_tone_length[harmonics_length] = { F, F, F, F };

float harmonics_comp[harmonics_length] = { C6, A5, C6, A5 };
float harmonics_comp_tone_length[harmonics_length] = { F, F, F, F };


#endif
