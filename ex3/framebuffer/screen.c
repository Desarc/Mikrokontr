#include "screen.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define MAX_IMAGE_SIZE 76800
#define TILE_SIZE_8 64
#define TILE_SIZE_16 256
#define TILE_SIZE_32 1024

int fbfd = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int screensize = 0;
char *fbp = 0;

int image[IMAGE_SIZE][3]; // first number here is 1024 pixels in my image, 3 is for RGB values
int crate_image[TILE_SIZE_16][3];
int player_image[TILE_SIZE_16][3];
int wall_image[TILE_SIZE_16][3];

volatile int *image_ptr;


int main() {
	open_screen_driver();
	
	load_sokoban_images();
	//generate_random_image();

	
    write_to_screen(player_image, 100, 100, 16, 16);	

    close_screen_driver();
    	
    return 0;
}

void load_sokoban_images(void) {
	//image_ptr = crate_image;
	read_image_data("crate.bmp", crate_image, 16, 16);
	image_ptr = player_image;
	read_image_data("player.bmp", image_ptr, 16, 16);
	image_ptr = wall_image;
	read_image_data("wall.bmp", image_ptr, 16, 16);

}

void read_image_data(char image_path[], int *image_ptr, int height, int width) {
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
		*image_ptr[0] = getc(streamIn);
		*image_ptr[1] = getc(streamIn);
		*image_ptr[2] = getc(streamIn);
		image_ptr++;
		printf("pixel %d : [%d,%d,%d]\n",i+1,image_ptr[0],image_ptr[1],image_ptr[2]);
	}
	fclose(streamIn);
}

void generate_random_image(void) {
	int i;
	for(i=0;i<IMAGE_SIZE/3;i++){    // foreach pixel
		
		image[i][2] = 0xff;  // use BMP 24bit with no alpha channel
		image[i][1] = 0x0;  // BMP uses BGR but we want RGB, grab byte-by-byte
		image[i][0] = 0x0;  // reverse-order array indexing fixes RGB issue...
		//printf("pixel %d : [%d,%d,%d]\n",i+1,image[i][0],image[i][1],image[i][2]);
	}
	for(i;i<IMAGE_SIZE*2/3;i++){    // foreach pixel
		
		image[i][2] = 0x0;  // use BMP 24bit with no alpha channel
		image[i][1] = 0xff;  // BMP uses BGR but we want RGB, grab byte-by-byte
		image[i][0] = 0x0;  // reverse-order array indexing fixes RGB issue...
		//printf("pixel %d : [%d,%d,%d]\n",i+1,image[i][0],image[i][1],image[i][2]);
	}
	for(i;i<IMAGE_SIZE;i++){    // foreach pixel
		
		image[i][2] = 0x0;  // use BMP 24bit with no alpha channel
		image[i][1] = 0x0;  // BMP uses BGR but we want RGB, grab byte-by-byte
		image[i][0] = 0xff;  // reverse-order array indexing fixes RGB issue...
		//printf("pixel %d : [%d,%d,%d]\n",i+1,image[i][0],image[i][1],image[i][2]);
	}
}


void write_to_screen(int *image_ptr, int posX, int posY, int height, int width) {
	
	//pixel_no = height*width;
	long int location = (posY*width)+posX;
	int i, j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			*(fbp+location) = *image_ptr[0]; //blue
			location++;
			*(fbp+location) = *image_ptr[1]; //green
			location++;
			*(fbp+location) = *image_ptr[2];; //red
			location++;
			image_ptr++; 			
		}
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
