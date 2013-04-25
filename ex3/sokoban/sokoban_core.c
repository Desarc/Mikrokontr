#include "sokoban_core.h"

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
	if (dir == PUSH_RIGHT || dir == RIGHT) dirX = 1;
	else if (dir == PUSH_LEFT || dir == LEFT) dirX = -1;
	else if (dir == PUSH_DOWN || dir == DOWN) dirY = 1;
	else if (dir == PUSH_UP || dir == UP) dirY = -1;
	if (!playing) return;
	if (!undo && !redo) {
		reset_undone_ptr();
		undone_length = 0;
	}
	int posX = getX(), posY = getY();
	int valid = validMove(posX, posY, posX+dirX, posY+dirY);
	if (valid) {
		int box = moveTo(posX, posY, posX+dirX, posY+dirY);
		if (!undo && !replaying) updatePath(dirX, dirY, box);
	}
	if (getRemaining() == 0) playing = 0;
}

/* keep track of all moves */
void updatePath(int dirX, int dirY, int box) {
	char move;
	if (dirX == 1) move = (box) ? PUSH_RIGHT : RIGHT;
	else if (dirX == -1) move = (box) ? PUSH_LEFT : LEFT;
	else if (dirY == 1) move = (box) ? PUSH_DOWN : DOWN;
	else if (dirY == -1) move = (box) ? PUSH_UP : UP;
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
	if (!playing || path_length == 0) return;
	path_ptr--;
	char dir = *path_ptr;
	path_length--;
	*undone_moves_ptr = dir;
	undone_moves_ptr++;
	undone_length++;
	if (dir == PUSH_RIGHT || dir == PUSH_LEFT || dir == PUSH_DOWN || dir == PUSH_UP) undoBox(dir);
	if (dir == PUSH_RIGHT || dir == RIGHT) dir = LEFT;
	else if (dir == PUSH_LEFT || dir == LEFT) dir = RIGHT;
	else if (dir == PUSH_DOWN || dir == DOWN) dir = UP;
	else if (dir == PUSH_UP || dir == UP) dir = DOWN;
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

