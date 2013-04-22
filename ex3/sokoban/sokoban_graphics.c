#include "sokoban_graphics.h"
#include "../framebuffer/screen.h"
#include "../leds/leds_control.h"
#include "../buttons/buttons_control.h"

volatile int completed = 0;

int main (int argc, char *argv[]) {
	
	open_screen_driver();
	open_led_driver();
	open_buttons_driver();
	load_sokoban_images();
	clear_screen();
	reset();
	printf("WELCOME TO SOKOBAN!\n\n");
	while (!completed) {
		paintLevel();
		int cmd = read_button_status();
		//printf("cmd: %i", cmd);
		if (cmd != NONE) {
			performAction(cmd);
			debounce();
		}
	}
	close_buttons_driver();
	close_led_driver();
	close_screen_driver();
}

void performAction(int cmd) {
	if (cmd == BUTTON5) move('u', 0, 0);
	else if (cmd == BUTTON7) move('l', 0, 0);
	else if (cmd == BUTTON6) move('d', 0, 0);
	else if (cmd == BUTTON4) move('r', 0, 0);
	else if (cmd == BUTTON3) undoLastMove();
	else if (cmd == BUTTON2) redoMove();
	else if (cmd == BUTTON1) {
		reset_leds();
		reset();		
	}
	else if (cmd == BUTTON0) {
		displayWin();
	}
	//printf("action performed");
}

void updateScreen(int x, int y, char tile) {
	display_tile(tile, x, y, 16);
}


void paintLevel(void) {
	volatile char *level_ptr = getLevel();
	int dimX = getDimX(), dimY = getDimY();
	int i, j;
	for (j = 0; j < dimY; j++) {
		for (i = 0; i < dimX; i++) {
			char tile = *level_ptr;
			display_tile(tile, i, j, 16);
			level_ptr++;
		}
		//printf("\n");
	}
}

void displayWin(void) {
	completed = 1;
	display_image(WIN);
}

