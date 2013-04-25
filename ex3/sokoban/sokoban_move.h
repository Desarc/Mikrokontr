#ifndef SOKOBAN_MOVE_H
#define SOKOBAN_MOVE_H

#include "sokoban_headers.h"

#define UP 'u'
#define DOWN 'd'
#define LEFT 'l'
#define RIGHT 'r'
#define PUSH_UP 'U'
#define PUSH_DOWN 'D'
#define PUSH_LEFT 'L'
#define PUSH_RIGHT 'R'

int validMove(int fromX, int fromY, int toX, int toY);
int moveTo(int fromX, int fromY, int toX, int toY);
void undoBox(char dir);

#endif
