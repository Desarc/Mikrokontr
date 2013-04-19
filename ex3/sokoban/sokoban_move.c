#include "sokoban_move.h"

int validMove(int fromX, int fromY, int toX, int toY) {
	int dimX = getDimX(), dimY = getDimY();
	if (toX >= dimX || toY >= dimY || toY < 0 || toX < 0) {
		return 0;
	}
	char next = getTile(toX, toY);
	if (next == WALL) {
		return 0;
	}
	if (next == MOVABLE || next == MOVABLE_ON_TARGET) {
		int newMovableY = toY+(toY-fromY);
		int newMovableX = toX+(toX-fromX);
		char newMovablePos = getTile(newMovableX, newMovableY);
		if (newMovablePos == WALL || newMovablePos == MOVABLE || newMovablePos == MOVABLE_ON_TARGET) {
			return 0;
		}
	}
	return 1;
}

int moveTo(int fromX, int fromY, int toX, int toY) {
	int box = 0;
	setTile(fromX, fromY, getGridTile(fromX, fromY));
	char next = getTile(toX, toY);
	if (next == TARGET) {
		setTile(toX, toY, MOVER_ON_TARGET);
	}
	else if (next == MOVABLE || next == MOVABLE_ON_TARGET) {
		box = 1;
		next = getGridTile(toX, toY);
		if (next == BLANK) {
			setTile(toX, toY, MOVER);
		}
		else if (next == TARGET) {
			setTile(toX, toY, MOVER_ON_TARGET);
			increaseRemaining();
		}
		int newMovableY = toY+(toY-fromY);
		int newMovableX = toX+(toX-fromX);
		char newMovablePos = getTile(newMovableX, newMovableY);
		if (newMovablePos == BLANK) {
			setTile(newMovableX, newMovableY, MOVABLE);
		}
		else if (newMovablePos == TARGET) {
			setTile(newMovableX, newMovableY, MOVABLE_ON_TARGET);
			decreaseRemaining();
		}
	}
	else {
		setTile(toX, toY, MOVER);
	}
	return box;
}

void undoBox(char dir) {
	int dirX = 0, dirY = 0;
	if (dir == 'R') {
		dirX = 1;
	}
	else if (dir == 'L') {
		dirX = -1;
	}
	else if (dir == 'D') {
		dirY = 1;
	}
	else if (dir == 'U') {
		dirY = -1;
	}
	int boxX = getX()+2*dirX;
	int boxY = getY()+2*dirY;
	if (getTile(boxX, boxY) == MOVABLE || getTile(boxX, boxY) == MOVABLE_ON_TARGET) {
		int newX = getX()+dirX;
		int newY = getY()+dirY;
		setTile(boxX, boxY, getGridTile(boxX, boxY));
		if (getGridTile(newX, newY) == TARGET) {
			setTile(newX, newY, MOVABLE_ON_TARGET);
		}
		else {
			setTile(newX, newY, MOVABLE);
		}
	}
}