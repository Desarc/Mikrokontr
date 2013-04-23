#include "leds_control.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

volatile long led_vector = 0x0;
volatile int n_of_leds = 0;
int fd_leds = 0;

/* increase the number of active LEDs */
/* this number represents the number of empty targets remaining, up to a maximum of 8 */
void increment_leds(void) {
	if (n_of_leds == 0) set_led_on(LED0);
	else if (n_of_leds == 1) set_led_on(LED1);
	else if (n_of_leds == 2) set_led_on(LED2);
	else if (n_of_leds == 3) set_led_on(LED3);
	else if (n_of_leds == 4) set_led_on(LED4);
	else if (n_of_leds == 5) set_led_on(LED5);
	else if (n_of_leds == 6) set_led_on(LED6);
	else if (n_of_leds == 7) set_led_on(LED7);
	if (n_of_leds < 8) n_of_leds++;
}

/* decrease the number of active LEDs */
void decrement_leds(void) {
	if (n_of_leds == 1) set_led_off(LED0);
	else if (n_of_leds == 2) set_led_off(LED1);
	else if (n_of_leds == 3) set_led_off(LED2);
	else if (n_of_leds == 4) set_led_off(LED3);
	else if (n_of_leds == 5) set_led_off(LED4);
	else if (n_of_leds == 6) set_led_off(LED5);
	else if (n_of_leds == 7) set_led_off(LED6);
	else if (n_of_leds == 8) set_led_off(LED7);
	if (n_of_leds > 0) n_of_leds--;
}

/* blink the LEDs to indicate the game is won */
void blink_leds(void) {

	/* fork process to avoid blocking */
	pid_t childPID = fork();
	
	if (childPID == 0) {
		led_vector = 0x0;
		int i;
		for (i=0;i<10;i++) {
			write_to_led_driver();
			if (i%2 == 0) led_vector = ALL_LEDS;
			else led_vector = 0x0; 
			int wait = 0xeffff;
			while (wait > 0) {
				wait--;
			}
		}
		write_to_led_driver();
		exit(1);
	}
}

int get_led_status(void) {
	return led_vector;
}

/* turn all LEDs off */
void reset_leds(void) {
	led_vector = 0x0;
	n_of_leds = 0;
	write_to_led_driver();
}

/* turn the selected LED on */
void set_led_on(int led) {
	led_vector += led;
	write_to_led_driver();
}

/* turn the selected LED off */
void set_led_off(int led) {
	led_vector -= led;
	write_to_led_driver();
}

/* write data to the led driver */
void write_to_led_driver(void) {
	char status[4];
	status[0] = led_vector >> 16;
	status[1] = led_vector >> 8;
	status[2] = led_vector;
	int write_success = write(fd_leds, status, 3);
	if (write_success == -1) {
        	perror("Error: cannot write to LED device\n");
        	exit(1);
   	}
}

/* open buttons driver */
void open_led_driver(void) {
	/* open the driver file for reading and writing */
	fd_leds = open("/dev/leds", O_RDWR);
    	if (fd_leds == -1) {
        	perror("Error: cannot open LED device\n");
        	exit(1);
   	}
    	printf("The LED device was opened successfully.\n");
}

/* close buttons driver */
void close_led_driver(void) {
    	close(fd_leds);
}
