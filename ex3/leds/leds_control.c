#include "leds_control.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

volatile int led_vector = 0x0;
volatile int n_of_leds = 0;
int fd_leds = 0;

/*int main (int argc, char *argv[]) {
	led_vector = 0;
	open_led_driver();
	set_led_on(LED0);
	set_led_on(LED1);
	set_led_on(LED2);
	set_led_on(LED3);
	set_led_on(LED4);
	set_led_on(LED5);
	set_led_on(LED6);
	close_led_driver();
}*/

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

void reset_leds(void) {
	led_vector = 0x0;
	n_of_leds = 0;
	write_to_led_driver();
}

void set_led_on(int led) {
	led_vector += led;
	write_to_led_driver();
}

void set_led_off(int led) {
	led_vector -= led;
	write_to_led_driver();
}

void write_to_led_driver(void) {
	int write_success = -1;
	write_success = write(fd_leds, &led_vector, 4);
	if (write_success == -1) {
        	perror("Error: cannot write to LED device\n");
        	exit(1);
   	}
}

void open_led_driver(void) {
	// Open the file for reading and writing
	fd_leds = open("/dev/leds", O_RDWR);
    	if (fd_leds == -1) {
        	perror("Error: cannot open LED device\n");
        	exit(1);
   	}
    	printf("The LED device was opened successfully.\n");
}


void close_led_driver(void) {
    	close(fd_leds);
}



