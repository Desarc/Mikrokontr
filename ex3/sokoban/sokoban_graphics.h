#ifndef SOKOBAN_GRAPHICS_H
#define SOKOBAN_GRAPHICS_H

#include "sokoban_headers.h"

void updateScreen(int x, int y, char tile);
void paintLevel(volatile char *level_ptr);



#endif