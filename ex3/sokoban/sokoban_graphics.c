#include "sokoban_graphics.h"

volatile int completed = 0;

int main (int argc, char *argv[]) {
	printf("WELCOME TO SOKOBAN!\n\n");
	reset();
	while (!completed) {
		paintLevel();
		char cmd;
		scanf("%c", &cmd);
		performAction(cmd);
	}
}

void performAction(char cmd) {
	if (cmd == 'w') {
		move('u', 0, 0);
	}
	else if (cmd == 'a') {
		move('l', 0, 0);
	}
	else if (cmd == 's') {
		move('d', 0, 0);
	}
	else if (cmd == 'd') {
		move('r', 0, 0);
	}
	else if (cmd == 'u') {
		undoLastMove();
	}
	else if (cmd == 'y') {
		redoMove();
	}
	else if (cmd == 'p') {
		printPath();
	}
	else if (cmd == 'r') {
		reset();
	}
	else if (cmd == 'x') {
		completed = 1;
	}
}

void updateScreen(int x, int y, char tile) {
	
}

void printPath(void) {
	volatile char *path_ptr = getPath();
	int moves = numberOfMoves();
	path_ptr -= moves;
	int i;
	for (i = 0; i < moves; i++) {
		printf("%c", *path_ptr);
		path_ptr++;
	}
	printf("\n");
}

void paintLevel(void) {
	volatile char *level_ptr = getLevel();
	int dimX = getDimX(), dimY = getDimY();
	int i, j;
	for (j = 0; j < dimY; j++) {
		for (i = 0; i < dimX; i++) {
			char tile = *level_ptr;
			printf("%c", tile);
			level_ptr++;
		}
		printf("\n");
	}
}

void displayWin(void) {
	completed = 1;
	printf("\n\nLevel completed!\n\n");
}