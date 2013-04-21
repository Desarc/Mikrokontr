#ifndef SOKOBAN_GRAPHICS_H
#define SOKOBAN_GRAPHICS_H

#include "sokoban_headers.h"

int main (int argc, char *argv[]);
static void performAction(char cmd);
void updateScreen(int x, int y, char tile);
void printPath(void);
void paintLevel(void);
void displayWin(void);



#endif

