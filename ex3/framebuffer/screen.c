#include "screen.h"
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

int image[MAX_IMAGE_SIZE][3]; // first number here is 1024 pixels in my image, 3 is for RGB values
int crate_image[TILE_SIZE_16][3];
int player_image[TILE_SIZE_16][3];
int wall_image[TILE_SIZE_16][3];


//volatile pixel *image_ptr;


int main() {
	open_screen_driver();
	
	load_sokoban_images();
	generate_random_image(image);

	clear_screen();
	
	display_tile(WALL, 5, 5, 16);
    	//write_to_screen(player_image, 100, 100, 16, 16);
	//write_to_screen(image, 0, 0, MAX_HEIGHT, MAX_WIDTH);	

    	close_screen_driver();
    	
    return 0;
}

void load_sokoban_images(void) {
	//image_ptr = crate_image;
	read_image_data("crate.bmp", crate_image, 16, 16);
	//image_ptr = player_image;
	read_image_data("player.bmp", player_image, 16, 16);
	//image_ptr = wall_image;
	read_image_data("wall.bmp", wall_image, 16, 16);

}

void read_image_data(char image_path[], pixel *image_ptr, int height, int width) {
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
		//printf("pixel %d : [%d,%d,%d]\n",i+1,*image_ptr[0],*image_ptr[1],*image_ptr[2]);
	}
	fclose(streamIn);
}

void generate_random_image(pixel *image_ptr) {
	int i;
	for(i=0;i<MAX_IMAGE_SIZE/3;i++){    // foreach pixel
		
		*image_ptr[2] = 0xff;  // use BMP 24bit with no alpha channel
		*image_ptr[1] = 0x0;  // BMP uses BGR but we want RGB, grab byte-by-byte
		*image_ptr[0] = 0x0;  // reverse-order array indexing fixes RGB issue...
		image_ptr++;
		//printf("pixel %d : [%d,%d,%d]\n",i+1,image[i][0],image[i][1],image[i][2]);
	}
	for(i;i<MAX_IMAGE_SIZE*2/3;i++){    // foreach pixel
		
		*image_ptr[2] = 0x0;  // use BMP 24bit with no alpha channel
		*image_ptr[1] = 0xff;  // BMP uses BGR but we want RGB, grab byte-by-byte
		*image_ptr[0] = 0x0;  // reverse-order array indexing fixes RGB issue...
		image_ptr++;
		//printf("pixel %d : [%d,%d,%d]\n",i+1,image[i][0],image[i][1],image[i][2]);
	}
	for(i;i<MAX_IMAGE_SIZE;i++){    // foreach pixel
		
		*image_ptr[2] = 0x0;  // use BMP 24bit with no alpha channel
		*image_ptr[1] = 0x0;  // BMP uses BGR but we want RGB, grab byte-by-byte
		*image_ptr[0] = 0xff;  // reverse-order array indexing fixes RGB issue...
		image_ptr++;
		//printf("pixel %d : [%d,%d,%d]\n",i+1,image[i][0],image[i][1],image[i][2]);
	}
}

void display_tile(char image, int tilePosX, int tilePosY, int dim) {
	pixel *image_ptr;
	if (image == WALL) {
		image_ptr = wall_image;
	}
	else if (image == MOVER) {
		image_ptr = player_image;
	}
	else if (image == MOVABLE) {
		image_ptr = crate_image;
	}
	int posX = tilePosX*dim, posY = tilePosY*dim;
	write_to_screen(image_ptr, posX, posY, dim, dim);
}

void write_to_screen(pixel *image_ptr, int posX, int posY, int height, int width) {
	
	//pixel_no = height*width;
	long int location;
	int i, j;
	for (i = 0; i < height; i++) {
		location = ((posY+i)*MAX_WIDTH+(posX))*3;
		for (j = 0; j < width; j++) {
			int blue = *image_ptr[0];
			int green = *image_ptr[1];
			int red = *image_ptr[2];
			*(fbp+location) = blue; //blue
			location++;
			*(fbp+location) = green; //green
			location++;
			*(fbp+location) = red; //red
			location++;
			image_ptr++; 
			printf("pixel %d : [%d,%d,%d]\n", i*width+j, blue, green, red);			
		}
	}
}

void clear_screen(void) {
	int location = 0;
	int i;
	for (i = 0; i < MAX_IMAGE_SIZE; i++) {
		*(fbp+location) = 0xff; //blue
		location++;
		*(fbp+location) = 0xff; //green
		location++;
		*(fbp+location) = 0xff; //red
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
