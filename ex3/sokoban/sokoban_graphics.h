#ifndef SOKOBAN_GRAPHICS_H
#define SOKOBAN_GRAPHICS_H

#include "sokoban_headers.h"

int main (int argc, char *argv[]);
int mainMenu(void);
static void performAction(int cmd);
void printPath(void);
void playGame(void);
void paintLevel(void);
void displaySplash(void);
void displayWin(void);

#endif
