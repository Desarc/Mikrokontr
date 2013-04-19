#include "sokoban_core.h"


volatile char path[100];
volatile char undone_moves[100];
volatile int current_level = 3;

volatile int path_length = 0;
volatile int undone_length = 0;
volatile int replaying = 0;
volatile int playing = 1;

volatile char *path_ptr = path;
volatile char *undone_moves_ptr = undone_moves;
	
void reset_path_ptr(void) {
	path_ptr = path;
}

void reset_undone_ptr(void) {
	undone_moves_ptr = undone_moves;
}
	
void move(char dir, int undo, int redo) {
	int dirX = 0, dirY = 0;
	if (dir == 'r' || dir == 'R') {
		dirX = 1;
	}
	else if (dir == 'l' || dir == 'L') {
		dirX = -1;
	}
	else if (dir == 'd' || dir == 'D') {
		dirY = 1;
	}
	else if (dir == 'u' || dir == 'U') {
		dirY = -1;
	}
	if (!playing) {
		return;
	}
	if (!undo && !redo) {
		reset_undone_ptr();
		undone_length = 0;
	}
	int posX = getX(), posY = getY();
	if (validMove(posX, posY, posX+dirX, posY+dirY)) {
		int box = moveTo(posX, posY, posX+dirX, posY+dirY);
		if (!undo && !replaying) {
			updatePath(dirX, dirY, box);
		}
	}
	if (getRemaining() == 0) {
		displayWin();
		playing = 0;
	}
}

void updatePath(int dirX, int dirY, int box) {
	char move;
	if (dirX == 1) {
		move = (box) ? 'R' : 'r';
	}
	else if (dirX == -1) {
		move = (box) ? 'L' : 'l';
	}
	else if (dirY == 1) {
		move = (box) ? 'D' : 'd';
	}
	else if (dirY == -1) {
		move = (box) ? 'U' : 'u';
	}
	*path_ptr = move;
	path_ptr++;
	path_length++;
}
	
volatile char *getPath() {
	return path_ptr;
}
	
void reset(void) {
	reset_path_ptr();
	reset_undone_ptr();
	path_length = 0;
	undone_length = 0;
	playing = 1;
	replaying = 0;
	loadLevel(current_level);
}
	
void setReplaying(int rep) {
	replaying = rep;
}
	
void setPlaying(int play) {
	playing = play;
}
	
void undoLastMove(void) {
	if (!playing || path_length == 0) {
		return;
	}
	path_ptr--;
	char dir = *path_ptr;
	path_length--;
	*undone_moves_ptr = dir;
	undone_moves_ptr++;
	undone_length++;
	if (dir == 'r' || dir == 'R') {
		dir = 'l';
	}
	else if (dir == 'l' || dir == 'L') {
		dir = 'r';
	}
	else if (dir == 'd' || dir == 'D') {
		dir = 'u';
	}
	else if (dir == 'u' || dir == 'U') {
		dir = 'd';
	}
	move(dir, 1, 0);
	undoBox(dir);
}
	
void redoMove(void) {
	if (undone_length > 0) {
		undone_moves_ptr--;
		char dir = *undone_moves_ptr;
		undone_length--;
		move(dir, 0, 1);
	}
}	

int numberOfMoves() {
	return path_length;
}