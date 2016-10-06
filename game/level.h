#ifndef LEVEL_H
#define LEVEL_H

#include "system.h"

#define LEVEL_WIDTH 20
#define LEVEL_HEIGHT 20
#define LEVEL_WALL_WIDTH 2

#define WALL_CHAR 'w'
#define WALL_VALUE 49
#define EMPTY_CHAR ' '
#define EMPTY_VALUE 0
#define BACKGROUND_CHAR 'b'
#define BACKGROUND_VALUE 'b'
#define PLAYER_CHAR 'p'
#define PLAYER_VALUE 'p'
#define ZOMBIE_CHAR 'z'
#define ZOMBIE_VALUE 'z'

void level_init(void);
uint8_t* level_get_display(uint8_t x_position, uint8_t y_position);

#endif