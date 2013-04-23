#include "sound.h"
#include "linux/soundcard.h"
#include "sys/mman.h"
#include "sys/ioctl.h"
#include "stdio.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "unistd.h"


#define MAX_SOUND_SAMPLES 112000
#define SAMPLES_8S 64000
#define SAMPLES_1S 8000

#define N_OF_FRAGS 16

char one_less[SAMPLES_1S];
int one_less_size;
char *one_less_pos;;
//char *one_less_counter;
char one_more[SAMPLES_1S];
int one_more_size;
char *one_more_pos;;
//char *one_more_counter;
char hit_wall[SAMPLES_1S];
int hit_wall_size;
char *hit_wall_pos;;
//char *hit_wall_counter;
char victory[SAMPLES_8S];
int victory_size;
char *victory_pos;
//char *victory_counter;
char intro[SAMPLES_8S];
int intro_size;
char *intro_pos;
//char *intro_counter;

int fd_dsp;

//int active_sound_size;
char *active_sound_size;
int frag_size;
char *next_ptr;
char *looping_ptr;
void *sound_addr = 0;
long sound_addr_start;
long sound_addr_end;
void *counters_addr = 0;


void play_sound(int code) {
	
	if (code == ONE_LESS && *one_less_pos == 255) {
		*one_less_pos = activate_sound(one_less, one_less_size);
		//*one_less_counter = 0;
	}
	else if (code == ONE_MORE && *one_more_pos == 255) {
		*one_more_pos = activate_sound(one_more, one_more_size);
		//*one_more_counter = 0;
	}
	else if (code == HIT_WALL && *hit_wall_pos == 255) {
		*hit_wall_pos = activate_sound(hit_wall, hit_wall_size);
		//*hit_wall_counter = 0;
	}
	else if (code == MUSIC && *intro_pos == 255) {
		//clear_sound();
		decompose_int(active_sound_size, intro_size);
		*intro_pos = activate_sound(intro, intro_size);
		//*intro_counter = 0;
	}
	else if (code == VICTORY && *victory_pos == 255) {
		//clear_sound();
		decompose_int(active_sound_size, victory_size);
		*victory_pos = activate_sound(victory, victory_size);
		//*victory_counter = 0;
	}
	else if (code == INTRO && *intro_pos == 255) {
		//clear_sound();
		decompose_int(active_sound_size, intro_size);
		*intro_pos = activate_sound(intro, intro_size);
		//*intro_counter = 0;
	}
	
}

void stop_sound(int code) {
	if (code == ONE_LESS && *one_less_pos != 255) {
		*one_less_pos = deactivate_sound(one_less, one_less_size, *one_less_pos);
		//*one_less_counter = 255;
	}
	else if (code == ONE_MORE && *one_more_pos != 255) {
		*one_more_pos = deactivate_sound(one_more, one_more_size, *one_more_pos);
		//*one_more_counter = 255;
	}
	else if (code == HIT_WALL && *hit_wall_pos != 255) {
		*hit_wall_pos = deactivate_sound(hit_wall, hit_wall_size, *hit_wall_pos);
		//*hit_wall_counter = 255;
	}
	else if (code == MUSIC && *intro_pos != 255) {
		clear_sound();
		//*intro_pos = deactivate_sound(intro, intro_size, *intro_pos);
		//*intro_counter = 255;
	}
	else if (code == VICTORY && *victory_pos != 255) {
		clear_sound();
		//*victory_pos = deactivate_sound(victory, victory_size, *victory_pos);
		//*victory_counter = 255;
	}
	else if (code == INTRO && *intro_pos != 255) {
		clear_sound();
		//*intro_pos = deactivate_sound(intro, intro_size, *intro_pos);
		//*intro_counter = 255;
	}
}

char activate_sound(char *sound_array_ptr, int size) {
	char offset = *next_ptr;
	char *active_sound_ptr = sound_addr+offset*frag_size;
	int i;
	for (i=0;i<size;i++) {
		char sample = *sound_array_ptr;
		*active_sound_ptr = sample/2 + (*active_sound_ptr)/2;
		active_sound_ptr++;
		sound_array_ptr++;
		/* wrap around */
		if ((long)active_sound_ptr > sound_addr_end) {
			active_sound_ptr = (char *)sound_addr_start;
		}
	}
	return offset;
}

char deactivate_sound(char *sound_array_ptr, int size, char offset) {
	if (offset == 255) {
		return;
	}
	char *active_sound_ptr = sound_addr+offset*frag_size;
	int i;
	for (i=0;i<size;i++) {
		char sample = *sound_array_ptr;
		*active_sound_ptr -= sample/2;
		*active_sound_ptr = *active_sound_ptr*2;
		active_sound_ptr++;
		sound_array_ptr++;
		/* wrap around */
		if ((long)active_sound_ptr > sound_addr_end) {
			active_sound_ptr = (char*)sound_addr_start;
		}
	}
	return -1;
}

void kill_sound_process(void) {
	*looping_ptr = 0;
}

void loop_sound(void) {
	*looping_ptr = 1;
	while (*looping_ptr) {
		char offset = *next_ptr;
		char *current_ptr = sound_addr+(offset*frag_size);
		offset++;
		deactivate_expired_sounds(offset);
		*next_ptr = offset%N_OF_FRAGS;
		int write_success = write(fd_dsp, current_ptr, frag_size);
		if (write_success < 0) {
			perror("Write: ");
		}
	}
}

void deactivate_expired_sounds(int offset) {
	if (offset == *one_less_pos) {
		stop_sound(ONE_LESS);
	}
	if (offset == *one_more_pos) {
		stop_sound(ONE_MORE);
	}
	if (offset == *hit_wall_pos) {
		stop_sound(HIT_WALL);
	}
	/*char check = *one_less_counter;
	if (check == N_OF_FRAGS-1) {
		stop_sound(ONE_LESS);
	}
	else if (check != 255) {
		*one_less_counter += 1;
	}
	check = *one_more_counter;
	if (check == N_OF_FRAGS-1) {
		stop_sound(ONE_MORE);
	}
	else if (check != 255) {
		*one_more_counter += 1;
	}
	check = *hit_wall_counter;
	if (check == N_OF_FRAGS-1) {
		stop_sound(HIT_WALL);		
	}
	else if (check != 255) {
		*hit_wall_counter += 1;
	}*/
}

void load_sokoban_sounds(void) {
	/* load all sokoban sounds from file */
	one_less_size = load_sound_from_file("on.wav", one_less);
	one_more_size = load_sound_from_file("off.wav", one_more);
	hit_wall_size = load_sound_from_file("wall.wav", hit_wall);
	victory_size = load_sound_from_file("victory.wav", victory);
	intro_size = load_sound_from_file("intro.wav", intro);
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
}

void map_shared_memory(void) {
	char bytes[4];
	decompose_int(bytes, intro_size);
	frag_size = intro_size/N_OF_FRAGS;

	size_t length_sound = intro_size;
	off_t offset = 0;
	int prot = (PROT_READ | PROT_WRITE );
	int flags = MAP_SHARED;
	int fd_sound = -1;
	/* creating shared files */
	fd_sound = open("./shared_sound", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd_sound <= 0){
		printf("File opening error ocurred. Exiting program.\n");
		exit(0);
	}
	if (lseek(fd_sound, length_sound-1, SEEK_SET) < 0) {
		printf("File seek error ocurred. Exiting program.\n");
		exit(0);
	}
	
	/* data is next, looping, one_less_counter, one_more_counter, hit_wall_counter, one_less_pos, one_more_pos, hit_wall_pos */
	//char counters[12] = {0,0,255,255,255,255,255,255,255,255,255,255};
	//size_t length_counters = 12;

	/* data is next, looping, one_less_pos, one_more_pos, hit_wall_pos, victory_pos, intro_pos, active_sound_size (decomposed into bytes) */
	char counters[11] = {0,0,255,255,255,255,255, bytes[0],bytes[1],bytes[2],bytes[3]};
	size_t length_counters = 11;
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
		next_ptr = counters_addr++;
		looping_ptr = counters_addr++;
		//one_less_counter = counters_addr++;
		//one_more_counter = counters_addr++;
		//hit_wall_counter = counters_addr++;
		//victory_counter = counters_addr++;
		//intro_counter = counters_addr++;
		one_less_pos = counters_addr++;
		one_more_pos = counters_addr++;
		hit_wall_pos = counters_addr++;
		victory_pos = counters_addr++;
		intro_pos = counters_addr++;
		active_sound_size = counters_addr;
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
		next_ptr = counters_addr++;
		looping_ptr = counters_addr++;
		//one_less_counter = counters_addr++;
		//one_more_counter = counters_addr++;
		//hit_wall_counter = counters_addr++;
		//victory_counter = counters_addr++;
		//intro_counter = counters_addr++;
		one_less_pos = counters_addr++;
		one_more_pos = counters_addr++;
		hit_wall_pos = counters_addr++;
		victory_pos = counters_addr++;
		intro_pos = counters_addr++;
		active_sound_size = counters_addr;
	}
	printf("Shared sound data memory mapped.\n");
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
