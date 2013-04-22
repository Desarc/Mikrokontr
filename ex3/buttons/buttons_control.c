#include "buttons_control.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "fcntl.h"
/*#include <sys/mman.h>
#include <sys/ioctl.h>
*/

int fd_buttons = 0;
volatile int button_status;

/*int main (int argc, char *argv[]) {

	open_buttons_driver();
	read_button_status();
	close_buttons_driver();
}
*/

int read_button_status(void) {
	char test [2];
	char *test_ptr = test;
	int read_success;
	read_success = read(fd_buttons, test_ptr, 2);
	button_status = (test[0] << 8)+test[1];
	int led_status = get_led_status();
	button_status -= led_status << 8;
	//printf("Read success? %i\n", read_success);
	//printf("Button vector: %i\n", button_status);
	return button_status;
}

void debounce(void) {
	long wait = 0x5fffff;
	while (wait > 0) {
		wait--;
	}
}


void open_buttons_driver(void) {
	// Open the file for reading and writing
	fd_buttons = open("/dev/buttons", O_RDWR);
    	if (fd_buttons == -1) {
        	perror("Error: cannot open button device\n");
        	exit(1);
   	}
    	printf("The button device was opened successfully.\n");
}


void close_buttons_driver(void) {
    	close(fd_buttons);
}

