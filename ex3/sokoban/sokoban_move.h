#ifndef SOKOBAN_MOVE_H
#define SOKOBAN_MOVE_H

#include "sokoban_headers.h"

int validMove(int fromX, int fromY, int toX, int toY);
int moveTo(int fromX, int fromY, int toX, int toY);

#endif