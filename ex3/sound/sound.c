#include "sound.h"
#include "linux/soundcard.h"
#include "sys/mman.h"
#include "sys/ioctl.h"
#include "stdio.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "unistd.h"

#define SAMPLES_60S 480000
#define SAMPLES_8S 64000
#define SAMPLES_1S 8000

#define N_OF_FRAGS 600

char one_less[SAMPLES_1S];
int one_less_size;
char *one_less_pos;;
char one_more[SAMPLES_1S];
int one_more_size;
char *one_more_pos;;
char hit_wall[SAMPLES_1S];
int hit_wall_size;
char *hit_wall_pos;;
char victory[SAMPLES_8S];
int victory_size;
char *victory_pos;
char intro[SAMPLES_60S];
int intro_size;
char *intro_pos;
char music[SAMPLES_60S];
int music_size;
char *music_pos;

int fd_dsp;

char *active_sound_size;
char *frag_size;
char *next_ptr;
char *looping_ptr;
void *sound_addr = 0;
long sound_addr_start;
long sound_addr_end;
void *counters_addr = 0;


void play_sound(int code) {
	
	if (code == ONE_LESS && build_int(one_less_pos) == -1) {
		int new_pos = activate_sound(one_less, one_less_size);
		decompose_int(one_less_pos, new_pos);
	}
	else if (code == ONE_MORE && build_int(one_more_pos) == -1) {
		int new_pos = activate_sound(one_more, one_more_size);
		decompose_int(one_more_pos, new_pos);
	}
	else if (code == HIT_WALL && build_int(hit_wall_pos) == -1) {
		int new_pos = activate_sound(hit_wall, hit_wall_size);
		decompose_int(hit_wall_pos, new_pos);
	}
	else if (code == MUSIC && build_int(intro_pos) == -1) {
		set_current_sound_size(music_size);
		int new_pos = activate_sound(music, music_size);
		decompose_int(music_pos, new_pos);
	}
	else if (code == VICTORY && build_int(victory_pos) == -1) {
		set_current_sound_size(victory_size);
		int new_pos = activate_sound(victory, victory_size);
		decompose_int(victory_pos, new_pos); 
	}
	else if (code == INTRO && build_int(intro_pos) == -1) {
		set_current_sound_size(intro_size);
		int new_pos = activate_sound(intro, intro_size);
		decompose_int(intro_pos, new_pos);
	}
	
}

void stop_sound(int code) {
	if (code == ONE_LESS && build_int(one_less_pos) != -1) {
		deactivate_sound(one_less, one_less_size, build_int(one_less_pos));
		decompose_int(one_less_pos, -1);
	}
	else if (code == ONE_MORE && build_int(one_more_pos) != -1) {
		deactivate_sound(one_more, one_more_size, build_int(one_more_pos));
		decompose_int(one_more_pos, -1);
	}
	else if (code == HIT_WALL && build_int(hit_wall_pos) != -1) {
		deactivate_sound(hit_wall, hit_wall_size, build_int(hit_wall_pos));
		decompose_int(hit_wall_pos, -1);
	}
	else if (code == MUSIC && build_int(intro_pos) != -1) {
		clear_sound();
		decompose_int(music_pos, -1);
	}
	else if (code == VICTORY && build_int(victory_pos) != -1) {
		clear_sound();
		decompose_int(victory_pos, -1);
	}
	else if (code == INTRO && build_int(intro_pos) != -1) {
		clear_sound();
		decompose_int(intro_pos, -1);
	}
}

void set_current_sound_size(int size) {
	decompose_int(active_sound_size, size);
	decompose_int(frag_size, size/N_OF_FRAGS);
}

int activate_sound(char *sound_array_ptr, int size) {
	char offset = *next_ptr;
	char *active_sound_ptr = sound_addr+offset*build_int(frag_size);
	int i;
	for (i=0;i<size;i++) {
		char sample = *sound_array_ptr;
		*active_sound_ptr = sample/2 + (*active_sound_ptr)/2;
		active_sound_ptr++;
		sound_array_ptr++;
		/* wrap around */
		if ((long)active_sound_ptr > sound_addr_end) {
			active_sound_ptr = sound_addr;
		}
	}
	return offset;
}

void deactivate_sound(char *sound_array_ptr, int size, int offset) {
	if (offset == -1) {
		return;
	}
	char *active_sound_ptr = sound_addr+offset*build_int(frag_size);
	int i;
	for (i=0;i<size;i++) {
		char sample = *sound_array_ptr;
		*active_sound_ptr -= sample/2;
		*active_sound_ptr = *active_sound_ptr*2;
		active_sound_ptr++;
		sound_array_ptr++;
		/* wrap around */
		if ((long)active_sound_ptr > sound_addr_end) {
			active_sound_ptr = sound_addr;
		}
	}
}

void kill_sound_process(void) {
	decompose_int(looping_ptr, 0);
}

void loop_sound(void) {
	*looping_ptr = 1;
	while (*looping_ptr) {
		int offset = build_int(next_ptr);
		char *current_ptr = sound_addr+(offset*build_int(frag_size));
		offset++;
		deactivate_expired_sounds(offset);
		*next_ptr = offset%N_OF_FRAGS;
		int write_success = write(fd_dsp, current_ptr, build_int(frag_size));
		if (write_success < 0) {
			perror("Write: ");
			exit(1);
		}
	}
}

void deactivate_expired_sounds(int offset) {
	if (offset == build_int(one_less_pos)) {
		stop_sound(ONE_LESS);
	}
	if (offset == build_int(one_more_pos)) {
		stop_sound(ONE_MORE);
	}
	if (offset == build_int(hit_wall_pos)) {
		stop_sound(HIT_WALL);
	}
}

void load_sokoban_sounds(void) {
	/* load all sokoban sounds from file */
	one_less_size = load_sound_from_file("on.wav", one_less);
	one_more_size = load_sound_from_file("off.wav", one_more);
	hit_wall_size = load_sound_from_file("wall.wav", hit_wall);
	victory_size = load_sound_from_file("victory.wav", victory);
	intro_size = load_sound_from_file("intro_long.wav", intro);
	music_size = load_sound_from_file("music_long.wav", music);
	printf("Sokoban sounds loaded.\n");
}


void clear_sound(void) {
	char *active_sound_ptr = sound_addr;
	int size = build_int(active_sound_size);
	int i;
	for (i=0;i<size;i++) {
		*active_sound_ptr = 0;
		active_sound_ptr++;
	}
}

int build_int(char *bytes) {
	int num = *bytes << 24;
	bytes++;
	num += *bytes << 16;
	bytes++;
	num += *bytes << 8;
	bytes++;
	num += *bytes;
	//bytes-=3;
	return num;
}

void decompose_int(char *bytes, int data) {
	
	*bytes = data >> 24;
	bytes++;
	*bytes = data >> 16;
	bytes++;
	*bytes = data >> 8;
	bytes++;
	*bytes = data;
	//bytes-=3;
}

void map_shared_memory(void) {
	char sound_size_bytes[4];
	decompose_int(sound_size_bytes, intro_size);
	char frag_size_bytes[4];
	decompose_int(frag_size_bytes, intro_size/N_OF_FRAGS);

	/* creating shared files */
	size_t length_sound = intro_size;
	off_t offset = 0;
	int prot = (PROT_READ | PROT_WRITE );
	int flags = MAP_SHARED;
	int fd_sound = -1;
	
	fd_sound = open("./shared_sound", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd_sound <= 0){
		printf("File opening error ocurred. Exiting program.\n");
		exit(0);
	}
	if (lseek(fd_sound, length_sound-1, SEEK_SET) < 0) {
		printf("File seek error ocurred. Exiting program.\n");
		exit(0);
	}
	/* write initial file data to set size files */
	char zeros[SAMPLES_60S];
	int i;
	for (i=0;i<length_sound;i++) {
		zeros[i] = 0;
	}
	write(fd_sound, zeros, length_sound);
	char next_bytes[4];
	decompose_int(next_bytes, -1);
	char one_less_bytes[4];
	decompose_int(one_less_bytes, -1);
	char one_more_bytes[4];
	decompose_int(one_more_bytes, -1);
	char hit_wall_bytes[4];
	decompose_int(hit_wall_bytes, -1);
	char victory_bytes[4];
	decompose_int(victory_bytes, -1);
	char intro_bytes[4];
	decompose_int(intro_bytes, -1);
	char music_bytes[4];
	decompose_int(music_bytes, -1);
	/* data is next fragment, looping, one_less_pos, one_more_pos, hit_wall_pos, victory_pos, intro_pos, music_pos, active_sound_size (decomposed into bytes) */
	char counters[37] = {next_bytes[0],next_bytes[1],next_bytes[2],next_bytes[3],0,one_less_bytes[0],one_less_bytes[1],one_less_bytes[2],one_less_bytes[3],one_more_bytes[0],one_more_bytes[1],one_more_bytes[2],one_more_bytes[3],hit_wall_bytes[0],hit_wall_bytes[1],hit_wall_bytes[2],hit_wall_bytes[3],victory_bytes[0],victory_bytes[1],victory_bytes[2],victory_bytes[3],intro_bytes[0],intro_bytes[1],intro_bytes[2],intro_bytes[3],music_bytes[0],music_bytes[1],music_bytes[2],music_bytes[3],sound_size_bytes[0],sound_size_bytes[1],sound_size_bytes[2],sound_size_bytes[3],frag_size_bytes[0],frag_size_bytes[1],frag_size_bytes[2],frag_size_bytes[3]};
	size_t length_counters = 37;
	int fd_counters = -1;

	fd_counters = open("./shared_counters", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd_counters <= 0){
		printf("File opening error ocurred. Exiting program.\n");
		exit(0);
	}
	write(fd_counters, counters, length_counters);
	if (lseek(fd_counters, length_counters-1, SEEK_SET) < 0) {
		printf("File seek error ocurred. Exiting program.\n");
		exit(0);
	}
	pid_t childPID = fork();

	/* child process */	
	if (childPID == 0) {
		/* mapping sound file */
		sound_addr = mmap(NULL, length_sound, prot, flags, fd_sound, offset);
		if (sound_addr <= 0) {
			printf("Child memory mapping error of sample data ocurred. Exiting program.\n");
			exit(0);
		}
		sound_addr_end = (long)sound_addr+length_sound;

		/* mapping counter file */
		counters_addr = mmap(NULL, length_counters, prot, flags, fd_counters, offset);
		if (counters_addr <= 0) {
			printf("Parent memory mapping error of counters ocurred. Exiting program.\n");
			exit(0);
		}
		map_pointers(counters_addr);
		loop_sound();
		exit(1);
	}

	/* parent process */
	else {
		/* mapping sound file */
		sound_addr = mmap(NULL, length_sound, prot, flags, fd_sound, offset);
		if (sound_addr <= 0) {
			printf("Parent memory mapping error of sample data ocurred. Exiting program.\n");
			exit(0);
		}
		sound_addr_end = (long)sound_addr+length_sound;

		/* mapping counter file */
		counters_addr = mmap(NULL, length_counters, prot, flags, fd_counters, offset);
		if (counters_addr <= 0) {
			printf("Parent memory mapping error of counters ocurred. Exiting program.\n");
			exit(0);
		}
		map_pointers(counters_addr);
	}
	printf("Shared sound data memory mapped.\n");
}

void map_pointers(void *counters_addr) {
	next_ptr = counters_addr+=3;
	looping_ptr = counters_addr++;
	one_less_pos = counters_addr+=3;
	one_more_pos = counters_addr+=3;
	hit_wall_pos = counters_addr+=3;
	victory_pos = counters_addr+=3;
	intro_pos = counters_addr+=3;
	music_pos = counters_addr+=3;
	active_sound_size = counters_addr+=3;
	frag_size = counters_addr;
}

/* load sound samples from a file to an array */
int load_sound_from_file(char file_path[], char *sound_array_ptr) {
	FILE *streamIn;
	streamIn = fopen(file_path, "r");
	if (streamIn == (FILE *)0){
		printf("File opening error ocurred. Exiting program.\n");
		exit(0);
	}
	int size;
	fseek(streamIn, 0L, SEEK_END);
	size = ftell(streamIn);
	fseek(streamIn, 0L, SEEK_SET);
	int i;
	for (i=0;i<size;i++) {
		*sound_array_ptr = getc(streamIn);
		sound_array_ptr++;
	}
	fclose(streamIn);
	return size;
}

/* write sound samples to sound device */
void write_sound_to_device(char *sound_array_ptr, int size) {
	int ready = -1;
	while (!ready) {
		ready = ioctl(fd_dsp, SOUND_PCM_SYNC, 0);
	}

	int write_success = write(fd_dsp, sound_array_ptr, size);
	if (write_success < 0) {
		perror("Write: ");
	}

	int sync_success = ioctl(fd_dsp, SOUND_PCM_SYNC, 0);	
	if (sync_success < 0) {
		perror("Sync: ");
	}
}

int open_sound_driver(void) {
	/* open driver file */
	fd_dsp = open("/dev/dsp", O_RDWR);
	
	if (fd_dsp < 0) {
		perror("open: ");
		return 1;
	}
	printf("The sound device was opened successfully.\n");

	/* set number of channels, sample size and sampling rate */
	int dsp_rate = 8000;
	int channels = 1;
	int sample_size = 8;

	ioctl(fd_dsp, SOUND_PCM_WRITE_CHANNELS, &channels);
	ioctl(fd_dsp, SOUND_PCM_WRITE_BITS, &sample_size);
	ioctl(fd_dsp, SOUND_PCM_WRITE_RATE, &dsp_rate);

}

void close_sound_driver(void) {
	close(fd_dsp);
}
