#ifndef SOKOBAN_LEVEL_H
#define SOKOBAN_LEVEL_H

#include "sokoban_headers.h"

volatile char *getLevel(void);
static void reset_level_ptr(void);
static void reset_grid_ptr(void);
char setTile(int x, int y, char tile);
char getTile(int x, int y);
char getGridTile(int x, int y);
int getRemaining(void);
void increaseRemaining(void);
void decreaseRemaining(void);
void loadLevel(int num);
int getX(void);
int getY(void);
int getDimX(void);
int getDimY(void);
void reset(int num);


#endif