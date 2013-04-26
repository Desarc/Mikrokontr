#include "buttons_control.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "fcntl.h"

int fd_buttons = 0;
volatile int button_status;

/* read current status of buttons */
int read_button_status(void) {
	char status [2];
	char *status_ptr = status;
	int read_success;
	read_success = read(fd_buttons, status_ptr, 2);
	button_status = (status[0] << 8)+status[1];
	int led_status = get_led_status();
	button_status -= led_status << 8;
	return button_status;
}

/* debounce to avoid repeat of button push */
void debounce(void) {
	long wait = 0x400000;
	while (wait > 0) {
		wait--;
	}
}

/* open buttons driver */
void open_buttons_driver(void) {
	/* open the driver file for reading and writing */
	fd_buttons = open("/dev/buttons", O_RDWR);
    	if (fd_buttons == -1) {
        	perror("Error: cannot open button device\n");
        	exit(1);
   	}
    	printf("The button device was opened successfully.\n");
}

/* close buttons driver */
void close_buttons_driver(void) {
    	close(fd_buttons);
}
