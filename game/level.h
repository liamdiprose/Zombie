#ifndef LEVEL_H
#define LEVEL_H

#include "system.h"
#include "point.h"

#define LEVEL_WIDTH 21
#define LEVEL_HEIGHT 21

// TODO: Remove "value" #defines? draw related, not level related
#define LEVEL_WALL 'w'
#define WALL_VALUE 30
#define LEVEL_EMPTY ' '
#define EMPTY_VALUE 0
#define BACKGROUND_CHAR 'b'
#define BACKGROUND_VALUE 1
#define LEVEL_PLAYER 'p'
#define PLAYER_VALUE 30
#define LEVEL_ZOMBIE 'z'
#define ZOMBIE_VALUE 50
#define NUM_ZOMBIE_GROUPS 5

// TODO: Confirm these directions
#define NAV_LEFT  -1, 0
#define NAV_RIGHT 1, 0
#define NAV_UP 0, 1
#define NAV_DOWN 0, -1


void level_init(char data[][]);

// Get the thing at the point
char level_get_point(point pt);

// Move zombie in direction if free space
bool nav_try_move(point pt, int8_t delta_x, int8_t delta_y);

// Try move zombie closer to the player
point nav_move_zombie(point zombie_location, point player_location);

// Update locations of a group of zombies (0 to NUM_ZOMBIE_GROUPS-1).
void nav_update_zombie_group(uint8_t group, point player_pos);



#endif
