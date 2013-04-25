#include "sokoban_level.h"
#include "sokoban_leveldefs.h"
#include "../leds/leds_control.h"
#include "../sound/sound.h"

volatile int posX, posY, dimX, dimY;
volatile int remaining;

#define maxTiles 200
volatile char level[maxTiles];
volatile char grid[maxTiles];

volatile char *level_ptr;
volatile char *grid_ptr;

volatile char *getLevel(void) {
	reset_level_ptr();
	return level_ptr;
}

void reset_level_ptr(void) {
	level_ptr = level;
}

void reset_grid_ptr(void) {
	grid_ptr = grid;
}

/* update a tile on the board */
char setTile(int x, int y, char tile) {
	char old = getTile(x, y);
	reset_level_ptr();
	int pos = (y*dimX)+x;
	level_ptr += pos;
	*level_ptr = tile;
	if (tile == PLAYER || tile == PLAYER_ON_TARGET) {
		posX = x;
		posY = y;
	}
	tile_updated(tile, x, y);
	return old;
}

/* get the character value of a tile on the board */
char getTile(int x, int y) {
	reset_level_ptr();
	int pos = (y*dimX)+x;
	level_ptr += pos;
	char tile = *level_ptr;
	return *level_ptr;
}

/* get the character value of a tile on the grid */
char getGridTile(int x, int y) {
	reset_grid_ptr();
	int pos = (y*dimX)+x;
	grid_ptr += pos;
	return *grid_ptr;
}

int getRemaining(void) {
	return remaining;
}

void increaseRemaining(void) {
	remaining++;
	increment_leds();
	play_sound(ONE_MORE);
}

void decreaseRemaining(void) {
	remaining--;
	decrement_leds();
	play_sound(ONE_LESS);
}

/* select level, load it to the array, and initialize fields */
void loadLevel(int num) {
	printf("Loading level %i.\n", num);
	remaining = 0;
	char *load_ptr;
	if (num == 1) {
		load_ptr = level1;
		dimX = level1dimX;
		dimY = level1dimY;
	}
	else if (num == 2) {
		load_ptr = level2;
		dimX = level2dimX;
		dimY = level2dimY;
	}
	else if (num == 3) {
		load_ptr = level3;
		dimX = level3dimX;
		dimY = level3dimY;
	}
	else if (num == 4) {
		load_ptr = level4;
		dimX = level4dimX;
		dimY = level4dimY;
	}
	else if (num == 5) {
		load_ptr = level5;
		dimX = level5dimX;
		dimY = level5dimY;
	}
	else if (num == 6) {
		load_ptr = level6;
		dimX = level6dimX;
		dimY = level6dimY;
	}
	else if (num == 7) {
		load_ptr = level7;
		dimX = level7dimX;
		dimY = level7dimY;
	}
	else if (num == 8) {
		load_ptr = level8;
		dimX = level8dimX;
		dimY = level8dimY;
	}
	reset_level_ptr();
	reset_grid_ptr();
	int tiles = dimX*dimY;
	int i;
	for (i = 0;i < tiles; i++) {
		char tile = *load_ptr;
		load_ptr++;
		*level_ptr = tile;
		level_ptr++;
		if (tile == BOX || tile == PLAYER) *grid_ptr = BLANK;
		else if (tile == BOX_ON_TARGET || tile == PLAYER_ON_TARGET) *grid_ptr = TARGET;
		else *grid_ptr = tile;
		grid_ptr++;
		if (tile == PLAYER) {
			posX = i%dimX;
			posY = (int)(i/dimX);
		}
		else if (tile == PLAYER_ON_TARGET) {
			posX = i%dimX;
			posY = (int)(i/dimX);
			remaining++;
			increment_leds();
		}
		else if (tile == TARGET) {
			remaining++;
			increment_leds();
		}
	}
	reset_level_ptr();
	reset_grid_ptr();
}

int getX(void) {
	return posX;
}

int getY(void) {
	return posY;
}

int getDimX(void) {
	return dimX;
}

int getDimY(void) {
	return dimY;
}

