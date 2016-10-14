/** @file   level.c
    @author Liam Diprose & Jeremy Craig
    @date   9 October 2016
    @brief  controls and stores zombie location
*/

#include "system.h"
#include "level.h"
#include "point.h"
#include "player.h"
#include "stdlib.h"
#include "protocol.h"

#define CLIENT_UPDATE_XPADDING 3        // Proximity of zombies to send to client
#define CLIENT_UPDATE_YPADDING 4

uint8_t p1_updated_zombies = 0;


void level_init(void)
{
    for (uint8_t x = 0; x < LEVEL_WIDTH; x++) {
        for (uint8_t y = 0; y < LEVEL_WIDTH; y++) {
            level_data[y][x] = LEVEL_EMPTY;
        }
    }
}

void level_set_zombie(uint8_t x, uint8_t y)
{
    static uint8_t previous_x = 0;
    static uint8_t previous_y = 0;

    if (y == 0) {

    } else if (y != previous_y) {
        for (uint8_t count = previous_x + 1; count < LEVEL_WIDTH; count++) {
            level_data[y][count] = LEVEL_EMPTY;
        }
        previous_y = y;
        previous_x = 0;
    }

    if (previous_x == LEVEL_WIDTH || x == 0) {

    } else if (x >= previous_x) {
        for (uint8_t count = previous_x + 1; count < x; count++) {
            level_data[previous_y][count] = LEVEL_EMPTY;
        }
        level_data[previous_y][x] = LEVEL_ZOMBIE;
        previous_x = x;
    } else {
        for (uint8_t count = previous_x + 1; count < LEVEL_WIDTH; count++) {
            level_data[y][count] = LEVEL_EMPTY;
        }
        previous_x = LEVEL_WIDTH;
    }

}

char level_get_point(Point pt)
{
    return level_data[pt.y][pt.x];
}

void level_set_point(Point pt, char givenChar)
{
    level_data[pt.y][pt.x] = givenChar;
}


void level_move(Point start, int8_t dest_x, int8_t dest_y)
{
    level_data[start.y][start.x] = LEVEL_EMPTY;
    bool hit_player = false;
    for (uint8_t player_id = 0; player_id <= 2; player_id++) {
        if (player_get_position(player_id).x == dest_x
            && player_get_position(player_id).y == dest_y) {
            hit_player = true;
            if (player_id == 0) {
                player_decrease_health();
            }
        }
    }
    if (hit_player) {
        level_data[start.y][start.x] = LEVEL_ZOMBIE;
    } else if (dest_x > start.x || dest_y > start.y) {
        level_data[dest_y][dest_x] = LEVEL_ZOMBIE_MOVED;
    } else {
        level_data[dest_y][dest_x] = LEVEL_ZOMBIE;
    }

}

// 
bool nav_try_move(Point zombie_position, int8_t dx, int8_t dy)
{
    Point canditate = { zombie_position.x + dx, zombie_position.y + dy };
    if (level_get_point(canditate) == LEVEL_EMPTY) {
        level_move(zombie_position, canditate.x, canditate.y);
        return true;
    } else {
        return false;
    }
}


// Move a single zombie towards the player. Do not move if cannot move closer in x or y direction.
void nav_move_zombie(Point zombie_pos, Point player_pos)
{
    bool attempted_sideways_move = false;
    bool has_moved = false;

    Point difference =
        { zombie_pos.x - player_pos.x, zombie_pos.y - player_pos.y };
    if (abs(difference.x) > abs(difference.y)) {
        // Try move sideways first 
        if (difference.x > 0) {
            // left
            // has_moved = nav_try_move(zombie_pos, (point) {-1, 0});
            has_moved = nav_try_move(zombie_pos, -1, 0);
        } else {
            // right
            //has_moved = nav_try_move(zombie_pos, (point) {1, 0});
            has_moved = nav_try_move(zombie_pos, 1, 0);
        }
        attempted_sideways_move = true;
    }
    if (!has_moved) {
        // Try move zombie up or down, if above failed or difference.y is greater 
        if (difference.y > 0) {
            // down
//            has_moved = nav_try_move(zombie_pos, (point) {0, 1});
            has_moved = nav_try_move(zombie_pos, 0, -1);
        } else {
            // up
//            has_moved = nav_try_move(zombie_pos, (point) {0, -1});
            has_moved = nav_try_move(zombie_pos, 0, 1);
        }

    }
    if (!has_moved && !attempted_sideways_move) {
        // Move left or right if hasnt tried yet, and moving up/down failed
        if (difference.x > 0) {
            // left
            // has_moved = nav_try_move(zombie_pos, (point) {-1, 0});
            has_moved = nav_try_move(zombie_pos, -1, 0);
        } else {
            // right
            //has_moved = nav_try_move(zombie_pos, (point) {1, 0});
            has_moved = nav_try_move(zombie_pos, 1, 0);
        }
    }
}

uint8_t distance(Point a, Point b)
{
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

// Update a group of zombies (TODO: group unsed for the time being, only if we need to 
// break up the execusion of this potienttally CPU-hog of a function
void nav_update_zombie_group(void *data)
{
    Player *players = data;
    for (int8_t row = 0; row < LEVEL_HEIGHT; row++) {
        for (int8_t col = 0; col < LEVEL_WIDTH; col++) {
            Point zombie = { col, row };
            if (level_data[row][col] == LEVEL_ZOMBIE) {
                if (distance(players[0].position, zombie) <
                    distance(players[1].position, zombie)) {
                    nav_move_zombie(zombie, players[0].position);
                } else {
                    nav_move_zombie(zombie, players[1].position);
                }
            } else if (level_get_point(zombie) == LEVEL_ZOMBIE_MOVED) {
                level_set_point(zombie, LEVEL_ZOMBIE);
            }
        }
    }
}


void level_update_client(__unused__ void *data)
{
    Point client_pos = players[1].position;
    uint8_t x_start = 0;
    uint8_t y_start = 0;

    uint8_t x_finish = LEVEL_WIDTH;
    uint8_t y_finish = LEVEL_HEIGHT;

    // Only send zombies in clients proximity
    if (client_pos.x > CLIENT_UPDATE_XPADDING) {
        x_start = client_pos.x - CLIENT_UPDATE_XPADDING;
    }
    if (client_pos.x < LEVEL_WIDTH - CLIENT_UPDATE_XPADDING) {
        x_finish = client_pos.x + CLIENT_UPDATE_XPADDING;
    }

    if (client_pos.y > CLIENT_UPDATE_YPADDING) {
        y_start = client_pos.y - CLIENT_UPDATE_YPADDING;
    }
    if (client_pos.y < LEVEL_WIDTH - CLIENT_UPDATE_YPADDING) {
        y_finish = client_pos.y + CLIENT_UPDATE_YPADDING;
    }

    uint8_t row;
    uint8_t col;

    for (col = x_start; col < x_finish; col++) {
        for (row = y_start; row < y_finish; row++) {
            if (level_data[row][col] == LEVEL_ZOMBIE) {
                protocol_write_zombie((Point) {
                                      col, row}
                );
            }
        }
    }
}

void level_add_zombie(__unused__ void* data){
    level_data[LEVEL_HEIGHT/2][LEVEL_WIDTH/2] = LEVEL_ZOMBIE;
}
