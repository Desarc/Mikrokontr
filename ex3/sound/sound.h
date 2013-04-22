#ifndef SOUND_H
#define SOUND_H

#define WELCOME 0
#define ONE_LESS 1
#define ONE_MORE 2
#define VICTORY 3
#define HIT_WALL 4

void play_sound(int code);
void load_sokoban_sounds(void);
static int load_sound_from_file(char file_path[], char *sound_array_ptr);
static void write_sound_to_device(char *sound_array_ptr, int size);
int open_sound_driver(void);
void close_sound_driver(void);


#endif
