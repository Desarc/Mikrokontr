#include "screen.h"
#include "../sokoban/sokoban_tile.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define MAX_IMAGE_SIZE 76800
#define MAX_HEIGHT 240
#define MAX_WIDTH 320
#define TILE_SIZE_8 64
#define TILE_SIZE_16 256
#define TILE_SIZE_32 1024

int fbfd = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int screensize = 0;
char *fbp = 0;

char win_image[MAX_IMAGE_SIZE][3];

char box_image[TILE_SIZE_16][3];
char player_image[TILE_SIZE_16][3];
char wall_image[TILE_SIZE_16][3];
char blank_image[TILE_SIZE_16][3];
char target_image[TILE_SIZE_16][3];
char player_on_target_image[TILE_SIZE_16][3];
char box_on_target_image[TILE_SIZE_16][3];

/*int main() {
	//test1(test);
	//test2(test);
	open_screen_driver();
	
	load_sokoban_images();
	generate_random_image(&image_array[0][0]);
	//read_image_data("24bit_flip_row_order.bmp", image_array, MAX_HEIGHT, MAX_WIDTH);
	clear_screen();
	
	display_tile(PLAYER, 5, 5, 16);
    	//write_to_screen(player_image, 100, 100, 16, 16);
	//write_to_screen(&image_array[0][0], 0, 0, MAX_HEIGHT, MAX_WIDTH);	

    	close_screen_driver();
    	
    return 0;
}*/

void load_sokoban_images(void) {
	read_image_data("crate.bmp", &box_image[0][0], 16, 16);
	read_image_data("player.bmp", &player_image[0][0], 16, 16);
	read_image_data("wall.bmp", &wall_image[0][0], 16, 16);
	read_image_data("blank.bmp", &blank_image[0][0], 16, 16);
	read_image_data("target.bmp", &target_image[0][0], 16, 16);
	read_image_data("playertarget.bmp", &player_on_target_image[0][0], 16, 16);
	read_image_data("cratetarget.bmp", &box_on_target_image[0][0], 16, 16);
	read_image_data("win.bmp", &win_image[0][0], MAX_HEIGHT, MAX_WIDTH);
}

void read_image_data(char image_path[], char *pixel_ptr, int height, int width) {
	FILE *streamIn;
	streamIn = fopen(image_path, "r");
	if (streamIn == (FILE *)0){
		printf("File opening error ocurred. Exiting program.\n");
		exit(0);
	}
	int size = width*height;
	int i;
	fseek(streamIn, 54, SEEK_SET);
	for(i=0;i<size;i++){
		*pixel_ptr = getc(streamIn);
		pixel_ptr++;
		*pixel_ptr = getc(streamIn);
		pixel_ptr++;
		*pixel_ptr = getc(streamIn);
		pixel_ptr++;
	}
	fclose(streamIn);
}

void generate_random_image(char *pixel_ptr) {
	int i;
	char blue = 0xff, green = 0x0, red = 0x0;
	for(i=0;i<MAX_IMAGE_SIZE;i++) {    // foreach pixel
		*pixel_ptr = blue;
		pixel_ptr++;
		*pixel_ptr = green;
		pixel_ptr++;
		*pixel_ptr = red;
		pixel_ptr++;
		if (i > MAX_IMAGE_SIZE*2/3) {
			green = 0x0;
			red = 0xff;
		}
		else if (i > MAX_IMAGE_SIZE/3) {
			blue = 0x0;
			green = 0xff;
		}
	}
}

void display_image(int image) {
	char *image_ptr;
	if (image == WIN) {
		image_ptr = &win_image[0][0];
	}
	write_to_screen(image_ptr, 0, 0, MAX_HEIGHT, MAX_WIDTH);
}

void display_tile(char image, int tilePosX, int tilePosY, int dim) {
	char *image_ptr;
	if (image == WALL) {
		image_ptr = &wall_image[0][0];
	}
	else if (image == PLAYER) {
		image_ptr = &player_image[0][0];
	}
	else if (image == BOX) {
		image_ptr = &box_image[0][0];
	}
	else if (image == BLANK) {
		image_ptr = &blank_image[0][0];
	}
	else if (image == TARGET) {
		image_ptr = &target_image[0][0];
	}
	else if (image == PLAYER_ON_TARGET) {
		image_ptr = &player_on_target_image[0][0];
	}
	else if (image == BOX_ON_TARGET) {
		image_ptr = &box_on_target_image[0][0];
	}
	int posX = tilePosX*dim, posY = tilePosY*dim;
	write_to_screen(image_ptr, posX, posY, dim, dim);
}

void write_to_screen(char *pixel_ptr, int posX, int posY, int height, int width) {
	long int location;
	int i, j;
	for (i = height; i >= 0; i--) {
		location = ((posY+i)*MAX_WIDTH+(posX))*3;
		for (j = 0; j < width; j++) {
			*(fbp+location) = *pixel_ptr; //blue
			location++;
			pixel_ptr++;
			*(fbp+location) = *pixel_ptr; //green
			location++;
			pixel_ptr++;
			*(fbp+location) = *pixel_ptr; //red
			location++;
			pixel_ptr++;
		}
	}
}

void clear_screen(void) {
	/* paint the whole screen white */
	int location = 0;
	int i;
	for (i = 0; i < MAX_IMAGE_SIZE; i++) {
		*(fbp+location) = 0xff;
		location++;
		*(fbp+location) = 0xff;
		location++;
		*(fbp+location) = 0xff;
		location++;		
	}
}

void open_screen_driver(void) {
	// Open the file for reading and writing
	fbfd = open("/dev/fb0", O_RDWR);
    	if (fbfd == -1) {
        	perror("Error: cannot open framebuffer device");
        	exit(1);
   	 }
    	printf("The framebuffer device was opened successfully.\n");

    	// Get fixed screen information
	int fscreen_success = ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo);
    	if ( fscreen_success == -1) {
        	perror("Error reading fixed information");
        	exit(2);
    	}

    	// Get variable screen information
	int vscreen_success = ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);
    	if (vscreen_success == -1) {
        	perror("Error reading variable information");
        	exit(3);
    	}

    	printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    	// Figure out the size of the screen in bytes
    	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    	// Map the device to memory
    	fbp = (char *)mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    	if ((int)fbp == -1) {
    	    perror("Error: failed to map framebuffer device to memory");
        	exit(4);
    	}
    	printf("The framebuffer device was mapped to memory successfully.\n");
}


void close_screen_driver(void) {
	munmap(fbp, screensize);
    	close(fbfd);
}
