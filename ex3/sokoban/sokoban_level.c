#include "sokoban_level.h"
#include "sokoban_leveldefs.h"

volatile int posX, posY, dimX, dimY;
volatile int remaining;

#define maxTiles 100
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
	updateScreen(x, y, tile);
	return old;
}

char getTile(int x, int y) {
	reset_level_ptr();
	int pos = (y*dimX)+x;
	level_ptr += pos;
	return *level_ptr;
}

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
}

void decreaseRemaining(void) {
	remaining--;
}

void loadLevel(int num) {
	remaining = 0;
	char *load_ptr;
	if (num == 3) {
		load_ptr = level3;
		dimX = level3dimX;
		dimY = level3dimY;
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
		if (tile == BOX || tile == PLAYER) {
			*grid_ptr = BLANK;
		}
		else if (tile == BOX_ON_TARGET || tile == PLAYER_ON_TARGET) {
			*grid_ptr = TARGET;
		}
		else {
			*grid_ptr = tile;
		}
		grid_ptr++;
		if (tile == PLAYER) {
			posX = i%dimY;
			posY = (int)(i/dimY);
		}
		else if (tile == PLAYER_ON_TARGET) {
			posX = i%dimY;
			posY = (int)(i/dimY);
			increaseRemaining();
		}
		else if (tile == TARGET) {
			increaseRemaining();
		}
	}
	reset_level_ptr();
	reset_grid_ptr();
	paintLevel();
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

