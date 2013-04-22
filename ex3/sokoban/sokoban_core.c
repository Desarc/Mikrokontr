#include "sokoban_core.h"
#include "../sound/sound.h"


volatile char path[100];
volatile char undone_moves[100];
volatile int current_level;

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

/* move in the direction 'dir' */	
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
	int valid = validMove(posX, posY, posX+dirX, posY+dirY);
	if (valid) {
		int box = moveTo(posX, posY, posX+dirX, posY+dirY);
		if (!undo && !replaying) {
			updatePath(dirX, dirY, box);
		}
		paintLevel();
	}
	else play_sound(HIT_WALL);
	if (getRemaining() == 0) {
		displayWin();
		playing = 0;
	}
}

/* keep track of all moves */
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

/* initialize fields and load the level specified */
void init_game(int num) {
	current_level = num;
	reset();
}

/* reset game */
void reset() {
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

/* undo the last move done */
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
	undoBox(dir);
	move(dir, 1, 0);
}
	
/* redo the last undone move */
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

