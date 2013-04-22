#ifndef SOUND_H
#define SOUND_H

int main (int argc, char *argv[]);
int open_driver(void);
void close_driver(void);
static void pregenerateTones(void);
static void generateSine(float f);
static void generateTriangle(float f);
void play_sound(int sound);
static void set_sample_size(float tone);
static void set_tone(float tone);
static void set_tone_length(float tone, float length);
static void init_sound(void);
void play_sample(void);


#endif
