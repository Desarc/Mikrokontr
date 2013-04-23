#ifndef SOKOBAN_GRAPHICS_H
#define SOKOBAN_GRAPHICS_H

#include "sokoban_headers.h"

int main (int argc, char *argv[]);
static void performAction(int cmd);
//void updateScreen(int x, int y, char tile);
int levelSelect(void);
void printPath(void);
void playGame(void);
void paintLevel(void);
void displaySplash(void);
void displayWin(void);

#endif
