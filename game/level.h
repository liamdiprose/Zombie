#ifndef LEVEL_H
#define LEVEL_H

#include "system.h"
#include "point.h"

#define LEVEL_WIDTH 20
#define LEVEL_HEIGHT 20

#define WALL_CHAR 'w'
#define WALL_VALUE 48
#define EMPTY_CHAR ' '
#define EMPTY_VALUE 0
#define BACKGROUND_CHAR 'b'
#define BACKGROUND_VALUE 1
#define PLAYER_CHAR 'p'
#define PLAYER_VALUE 30
#define ZOMBIE_CHAR 'z'
#define ZOMBIE_VALUE 50

void level_init(char data[][LEVEL_WIDTH]);

#endif