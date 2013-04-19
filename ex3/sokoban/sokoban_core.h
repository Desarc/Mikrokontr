#ifndef SOKOBAN_CORE_H
#define SOKOBAN_CORE_H

#include "sokoban_headers.h"

void reset_path_ptr(void);
void reset_undone_ptr(void);
void move(char dir, int undo, int redo);
static void updatePath(int dirX, int dirY, int box);
volatile char *getPath();
void reset(void);
void setReplaying(int rep);
void setPlaying(int play);
void undoLastMove(void);
void redoMove(void);
int numberOfMoves();





#endif