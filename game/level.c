#include "system.h"
#include "level.h"
#include "point.h"


char level_data[LEVEL_HEIGHT][LEVEL_WIDTH];

void level_init(char data[][LEVEL_WIDTH])
{
    /*int x; int y;
       for (x = 0; x < LEVEL_WIDTH; x++){
       for (y = 0; y < LEVEL_WIDTH; y++){

       if ((y%4) == 0 && (x%4) == 0){
       data[y][x] = BACKGROUND_CHAR;
       } else {
       data[y][x] = EMPTY_CHAR;
       }


       }
       } */

}


char level_get_point(point pt)
{
    return level_data[pt.y][pt.x];
}

void level_set_point(point pt, char thing)
{
    level_data[pt.y][pt.x] = thing;
}

point level_get_opponent()
{

}

void level_move(point start, point dest)
{
    level_set_point(dest, level_get_point(start));
    level_set_point(start, LEVEL_NULL);
}

// 
bool nav_try_move(point pt, int8_t delta_x, int8_t delta_y)
{
    point canditate =
        (point) { zombie_pos.y + delta_y, zombie_pos.x + delta_x };
    if (level_get_point(canditate) == LEVEL_EMPTY) {
        level_move(zombie_pos, canditate);
        return true;
    } else {
        return false;
    }
}


// Move a single zombie towards the player. Do not move if cannot move closer in x or y direction.
void nav_move_zombie(point zombie_pos, point player_pos)
{
    // 1. If difference in x is more than difference in y
    bool attempted_sideways_move = false;
    bool has_moved = false;
    if (abs(zombie_pos.x - player_pos.x) >
        abs(zombie_pos.y - player_pos.y)) {
        // 2. Try move the zombie laterlly
        if (zombie_pos.x > player_pos.x) {
            // Try move zombie to the left 
            has_moved = nav_try_move(zombie_pos, NAV_LEFT);
        } else {
            has_moved = nav_try_move(zombie_pos, NAV_RIGHT);
        }
        attempted_sideways_move = true;
    }
    if (!has_moved) {
        // Try move zombie up or down, 
        if (zombie_pos.y > player_pos.y) {
            // Try move zombie to the left 
            has_moved = nav_try_move(zombie_pos, NAV_DOWN);
        } else {
            has_moved = nav_try_move(zombie_pos, NAV_UP);
        }

    }
    // TODO: Repeated code 
    if (!has_moved && !attempted_sideways_move) {
        // Attempt sideways move again
        if (zombie_pos.x > player_pos.x) {
            // Try move zombie to the left 
            has_moved = nav_try_move(zombie_pos, NAV_LEFT);
        } else {
            has_moved = nav_try_move(zombie_pos, NAV_RIGHT);
        }
    }
}


// Update a group of zombies (TODO: group unsed for the time being, only if we need to 
// break up the execusion of this potienttally CPU-hog of a function
void nav_update_zombie_group(uint8_t group, point player_pos)
{
    // TODO: Implementups
    for (uint8_t row = 0; row < LEVEL_WIDTH; row++) {
        for (uint8_t col = 0; col < LEVEL_HEIGHT; col++) {
            if (level_get_point((point) {
                                col, row}
                ) == LEVEL_ZOMBIE) {
                nav_move_zombie((point) {
                                col, row}
                                , player_pos);
            }
        }
    }
}
