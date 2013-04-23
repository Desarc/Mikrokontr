#include "sokoban_move.h"

/* check if a move is valid */
int validMove(int fromX, int fromY, int toX, int toY) {
	int dimX = getDimX(), dimY = getDimY();
	if (toX >= dimX || toY >= dimY || toY < 0 || toX < 0) {
		return 0;
	}
	char next = getTile(toX, toY);
	if (next == WALL) {
		return 0;
	}
	if (next == BOX || next == BOX_ON_TARGET) {
		int newMovableY = toY+(toY-fromY);
		int newMovableX = toX+(toX-fromX);
		char newMovablePos = getTile(newMovableX, newMovableY);
		if (newMovablePos == WALL || newMovablePos == BOX || newMovablePos == BOX_ON_TARGET) {
			return 0;
		}
	}
	return 1;
}

/* move player (and box) from one position to another on the board */
int moveTo(int fromX, int fromY, int toX, int toY) {
	int box = 0;
	setTile(fromX, fromY, getGridTile(fromX, fromY));
	char next = getTile(toX, toY);
	if (next == TARGET) {
		setTile(toX, toY, PLAYER_ON_TARGET);
	}
	else if (next == BOX || next == BOX_ON_TARGET) {
		box = 1;
		next = getGridTile(toX, toY);
		if (next == BLANK) {
			setTile(toX, toY, PLAYER);
		}
		else if (next == TARGET) {
			setTile(toX, toY, PLAYER_ON_TARGET);
			increaseRemaining();
		}
		int newMovableY = toY+(toY-fromY);
		int newMovableX = toX+(toX-fromX);
		char newMovablePos = getTile(newMovableX, newMovableY);
		if (newMovablePos == BLANK) {
			setTile(newMovableX, newMovableY, BOX);
		}
		else if (newMovablePos == TARGET) {
			setTile(newMovableX, newMovableY, BOX_ON_TARGET);
			decreaseRemaining();
		}
	}
	else if (next == BLANK) {
		setTile(toX, toY, PLAYER);
	}
	return box;
}

/* undo a box push */
void undoBox(char dir) {
	int dirX = 0, dirY = 0;
	if (dir == 'R' || dir == 'r') {
		dirX = -1;
	}
	else if (dir == 'L' || dir == 'l') {
		dirX = 1;
	}
	else if (dir == 'D' || dir == 'd') {
		dirY = -1;
	}
	else if (dir == 'U' || dir == 'u') {
		dirY = 1;
	}
	int boxX = getX()+2*dirX;
	int boxY = getY()+2*dirY;
	if (getTile(boxX, boxY) == BOX || getTile(boxX, boxY) == BOX_ON_TARGET) {
		int newX = getX()+dirX;
		int newY = getY()+dirY;
		if (getTile(boxX, boxY) == BOX_ON_TARGET) {
			increaseRemaining();
		}
		setTile(boxX, boxY, getGridTile(boxX, boxY));
		if (getGridTile(newX, newY) == TARGET) {
			decreaseRemaining();
			setTile(newX, newY, BOX_ON_TARGET);
		}
		else {
			setTile(newX, newY, BOX);
		}	
	}
}

