#ifndef SOUND_H
#define SOUND_H

#define WELCOME 0
#define ONE_LESS 1
#define ONE_MORE 2
#define VICTORY 3
#define HIT_WALL 4
#define INTRO 5
#define MUSIC 6

void play_sound(int code);
void stop_sound(int code);
static char activate_sound(char *sound_array_ptr, int size);
static char deactivate_sound(char *sound_array_ptr, int size, char offset);
void clear_sound(void);
static void set_current_sound_size(int size);
static void deactivate_expired_sounds(int offset);
void load_sokoban_sounds(void);
static void loop_sound();
static void decompose_int(char *bytes, int data);
static int build_int(char *bytes);
void kill_sound_process(void);
void map_shared_memory(void);
static int load_sound_from_file(char file_path[], char *sound_array_ptr);
static void write_sound_to_device(char *sound_array_ptr, int size);
int open_sound_driver(void);
void close_sound_driver(void);

#endif
