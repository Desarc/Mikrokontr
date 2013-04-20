#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

int main() {
	int fbfd = 0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long int screensize = 0;
	char *fbp = 0;
	int x = 0, y = 0;
	long int location = 0;

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
	location = 0;
	for (x = 0; x < vinfo.xres; x++) {
		for (y = 0; y < vinfo.yres; y++) {
			*(fbp+location) = 0xff; //blue
			location++;
			*(fbp+location) = 0x0; //green
			location++;
			*(fbp+location) = 0x0; //red
			location++;
 			
		}
	}
    	
    return 0;
}

void open_driver(void) {

}


void close_driver(void) {
	munmap(fbp, screensize);
    	close(fbfd);
}
