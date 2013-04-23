#include "sokoban_graphics.h"
#include "../framebuffer/screen.h"
#include "../leds/leds_control.h"
#include "../buttons/buttons_control.h"
#include "../sound/sound.h"

volatile int completed = 0;

int main (int argc, char *argv[]) {
	/* open all drivers */
	open_screen_driver();
	open_led_driver();
	open_buttons_driver();
	open_sound_driver();

	/* load images and sounds */
	load_sokoban_images();
	load_sokoban_sounds();

	/* initialize game and screen */
	clear_screen();
	reset_leds();
	printf("\nWELCOME TO SOKOBAN!\n");

	/* game loop */
	playGame();

	/* close all drivers */
	close_sound_driver();
	close_buttons_driver();
	close_led_driver();
	close_screen_driver();
}

void playGame(void) {
	/* clean up if you choose to play again */
	debounce();
	reset_leds();
	clear_screen();

	/* level select */
	int level = mainMenu();
	init_game(level);
	play_sound(WELCOME);
	clear_screen();
	paintLevel();
	debounce();

	/* game loop */
 	/* busy waiting until a button is pushed */
	while (!completed) {
		int cmd = read_button_status();
		if (cmd != NONE) {
			performAction(cmd);
			paintLevel();
			debounce();
		}
	}

	/* press button 0 to exit program */
	/* (safeguard to avoid forked processes writing to drivers after they close) */
	int exit = 0;
	while (!exit) {
		int cmd = read_button_status();
		if (cmd == BUTTON0) {
			printf("\nExiting game.\n");
			exit = 1;
		}
	}
}

int mainMenu(void) {
	/* level select */
	printf("Please choose a level.\n\n");
	play_sound(INTRO);
	display_image(SPLASH);
	int chosen = 0;
	int choice;
	while (!chosen) {
		int cmd = read_button_status();
		if (cmd != NONE) {
			choice = chooseLevel(cmd);
			chosen = 1;
		}
	}
}

/* main menu button actions */
int chooseLevel(int cmd) {
	int choice;
	if (cmd == BUTTON0) choice = 1;
	else if (cmd == BUTTON1) choice = 2;
	else if (cmd == BUTTON2) choice = 3;
	else if (cmd == BUTTON3) choice = 4;
	else if (cmd == BUTTON4) choice = 5;
	else if (cmd == BUTTON5) choice = 6;
	else if (cmd == BUTTON6) choice = 7;
	return choice;
}

/* in-game button actions */
void performAction(int cmd) {
	if (cmd == BUTTON5) move('u', 0, 0);
	else if (cmd == BUTTON7) move('l', 0, 0);
	else if (cmd == BUTTON6) move('d', 0, 0);
	else if (cmd == BUTTON4) move('r', 0, 0);
	else if (cmd == BUTTON3) undoLastMove();
	else if (cmd == BUTTON2) redoMove();
	else if (cmd == BUTTON1) {
		/* reset level */
		clear_screen();
		reset_leds();
		reset();		
	}
	else if (cmd == BUTTON0) {
		/* go to main menu */
		playGame();
	}
}

void paintLevel(void) {
	/* paint level on screen */
	volatile char *level_ptr = getLevel();
	int dimX = getDimX(), dimY = getDimY();
	int i, j;
	for (j = 0; j < dimY; j++) {
		for (i = 0; i < dimX; i++) {
			char tile = *level_ptr;
			display_tile(tile, i, j, 16);
			level_ptr++;
		}
	}
}

void displayWin(void) {
	/* display win screen */
	completed = 1;
	display_image(WIN);
	blink_leds();
	play_sound(VICTORY);

	/* check if you want to play again */
	while (completed) {
		int cmd = read_button_status();
		int choice;
		if (cmd == BUTTON0) {
			completed = 0;
			playGame();
		}
	}
}
