#include "screen.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define IMAGE_SIZE 76800

int fbfd = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int screensize = 0;
char *fbp = 0;

int image[IMAGE_SIZE][3]; // first number here is 1024 pixels in my image, 3 is for RGB values

int main() {
	open_driver();
	
	read_image_data("gradient100x200.bmp");
	//generate_random_image();

	write_to_screen();


	
    	close_driver();	
    	
    return 0;
}

void read_image_data(char image_path[]) {
	FILE *streamIn;
	streamIn = fopen(image_path, "r");
	if (streamIn == (FILE *)0){
		printf("File opening error ocurred. Exiting program.\n");
		exit(0);
	}
	
	volatile int byte, width = 0, height = 0, size = 0, offset = 0, bpp = 0;
	int count = 0;
	int i;
	fseek(streamIn, 2, SEEK_SET);
	offset = getc(streamIn)+(getc(streamIn)+0xff);
	fseek(streamIn, 18, SEEK_SET);
	//int width1 = getc(streamIn)+0xff;
	//int width2 = getc(streamIn);
	width = getc(streamIn)+(getc(streamIn));
	fseek(streamIn, 22, SEEK_SET);
	//int height1 = getc(streamIn);
	//int height2 = getc(streamIn)+0xff;
	height = getc(streamIn)+(getc(streamIn));
	fseek(streamIn, 28, SEEK_SET);
	bpp = getc(streamIn);
	fseek(streamIn, 34, SEEK_SET);
	//int size1 = getc(streamIn)+0xff;
	//int size2 = getc(streamIn);
	size = (getc(streamIn)+(getc(streamIn)+0xff)+(getc(streamIn)+0xffff)+(getc(streamIn)+0xffffff))/(3*bpp);
	/*for(i=0;i<27;i++) {
		byte = getw(streamIn);  // strip out BMP header	
		printf("%i: %i\n", (i*2)+1,byte);
	}*/
	//for(i=0;i<11;i++) byte = getc(streamIn);  //read DIB header to width field
	/*fseek(streamIn, 18, SEEK_SET);
	width = getw(streamIn);
	height = getw(streamIn);
	//byte = getw(streamIn);
	
	byte = getw(streamIn);
	for(i=0;i<7;i++) byte = getc(streamIn);  //read DIB header to width field
	byte = getw(streamIn);
	size = getw(streamIn);*/
	printf("height: %i, width: %i, size: %i, bpp: %i, offset: %i\n", height, width, size, bpp, offset);
	//for(i=0;i<12;i++) byte = getc(streamIn);
	fseek(streamIn, offset, SEEK_SET);
	for(i=0;i<IMAGE_SIZE;i++){    // foreach pixel
		image[i][0] = getc(streamIn);  // use BMP 24bit with no alpha channel
		image[i][1] = getc(streamIn);  // BMP uses BGR but we want RGB, grab byte-by-byte
		image[i][2] = getc(streamIn);  // reverse-order array indexing fixes RGB issue...
		//byte = getc(streamIn);
		//printf("pixel %d : [%d,%d,%d]\n",i+1,image[i][0],image[i][1],image[i][2]);
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


void write_to_screen(void) {
	
	int x = 0, y = 0, pixel_no = IMAGE_SIZE;
	long int location = 0;
	int i;
	for (i = 0; i < IMAGE_SIZE; i++) {
		*(fbp+location) = image[pixel_no][0]; //blue
		location++;
		*(fbp+location) = image[pixel_no][1]; //green
		location++;
		*(fbp+location) = image[pixel_no][2];; //red
		location++;
		pixel_no--; 			
	}
}

void open_driver(void) {
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


void close_driver(void) {
	munmap(fbp, screensize);
    	close(fbfd);
}
