#include "sokoban_graphics.h"
#include "../framebuffer/screen.h"
#include "../leds/leds_control.h"
#include "../buttons/buttons_control.h"
#include "../sound/sound.h"

volatile int completed;

int main (int argc, char *argv[]) {
	/* open all drivers */
	open_screen_driver();
	open_led_driver();
	open_buttons_driver();
	open_sound_driver();

	/* load images and sounds */
	load_sokoban_images();
	load_sokoban_sounds();
	map_shared_memory();
	clear_sound();
	/* initialize game and screen */
	printf("\nWELCOME TO SOKOBAN!\n");

	/* game loop */
	int playing = 1;
	while (playing) {
		playGame();
		/* check if you want to play again */
		int decided = 0;
		while (!decided) {
			int cmd = read_button_status();
			if (cmd == BUTTON7) {
				playing = 0;
				decided = 1;
				debounce();
			}
			else if (cmd == BUTTON0) {
				decided = 1;
				debounce();
				stop_sound(VICTORY);
			}
		}	
	}
	display_image(END);
	kill_sound_process();
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
	clear_screen();

	/* close all drivers */
	close_sound_driver();
	close_buttons_driver();
	close_led_driver();
	close_screen_driver();
}

void playGame(void) {

	/* level select */
	int level = mainMenu();
	init_game(level);
	paintLevel();
	play_sound(MUSIC);
	completed = 0;
	/* game loop */
 	/* busy waiting until a button is pushed */
	while (!completed) {
		int cmd = read_button_status();
		if (cmd != NONE) {
			performAction(cmd);
			debounce();
			int remaining = getRemaining();
			if (remaining == 0) {
				completed = 1;
				displayWin();
			}
		}
	}
}

void tile_updated(char tile, int x, int y) {
	display_tile(tile, x, y, TILE_DIM);
}

int mainMenu(void) {
	/* level select */
	reset_leds();
	stop_sound(VICTORY);
	play_sound(INTRO);
	display_image(SPLASH);
	int chosen = 0;
	int choice;
	while (!chosen) {
		int cmd = read_button_status();
		if (cmd != NONE) {
			choice = chooseLevel(cmd);
			chosen = 1;
			debounce();
		}
	}
	stop_sound(INTRO);
	return choice;
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
	else if (cmd == BUTTON7) choice = 8;
	else choice = 1;
	return choice;
}

/* in-game button actions */
void performAction(int cmd) {
	if (cmd == BUTTON5) move(UP, 0, 0);
	else if (cmd == BUTTON7) move(LEFT, 0, 0);
	else if (cmd == BUTTON6) move(DOWN, 0, 0);
	else if (cmd == BUTTON4) move(RIGHT, 0, 0);
	else if (cmd == BUTTON3) undoLastMove();
	else if (cmd == BUTTON2) redoMove();
	else if (cmd == BUTTON1) {
		/* reset level */
		reset_leds();
		reset();
		paintLevel();	
	}
	else if (cmd == BUTTON0) {
		/* go to main menu */
		completed = 1;
		stop_sound(MUSIC);
	}
}

void paintLevel(void) {
	/* paint level on screen */
	clear_screen();
	volatile char *level_ptr = getLevel();
	int dimX = getDimX(), dimY = getDimY();
	int i, j;
	for (j = 0; j < dimY; j++) {
		for (i = 0; i < dimX; i++) {
			char tile = *level_ptr;
			display_tile(tile, i, j, TILE_DIM);
			level_ptr++;
		}
	}
}

void displayWin(void) {
	/* display win screen */
	printf("You win!\n");
	stop_sound(MUSIC);
	play_sound(VICTORY);
	display_image(WIN);
	blink_leds();
}
