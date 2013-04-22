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
#define SAMPLES 16000

char welcome[SAMPLES];
int welcome_size;
char one_less[SAMPLES];
int one_less_size;
char one_more[SAMPLES];
int one_more_size;
char hit_wall[SAMPLES];
int hit_wall_size;
char victory[MAX_SOUND_SAMPLES];
int victory_size;


int fd_dsp;

void play_sound(int code) {

	/* forking process to avoid blocking while sound is playing */
	pid_t childPID = fork();
	
	if (childPID == 0) {
		if (code == WELCOME) write_sound_to_device(welcome, welcome_size);
		else if (code == ONE_LESS) write_sound_to_device(one_less, one_less_size);
		else if (code == ONE_MORE) write_sound_to_device(one_more, one_more_size);
		else if (code == HIT_WALL) write_sound_to_device(hit_wall, hit_wall_size);
		else if (code == VICTORY) write_sound_to_device(victory, victory_size);
		exit(1);
	}
}

void load_sokoban_sounds(void) {
	
	/* load all sokoban sounds from file */
	welcome_size = load_sound_from_file("dudu.wav", welcome);
	one_less_size = load_sound_from_file("on.wav", one_less);
	one_more_size = load_sound_from_file("off.wav", one_more);
	hit_wall_size = load_sound_from_file("wall.wav", hit_wall);
	victory_size = load_sound_from_file("soundfile.wav", victory);
	printf("Sokoban sounds loaded.\n");

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
