/** @file   player.c
    @author Liam Diprose & Jeremy Craig
    @date   9 October 2016
    @brief  controls and stores zombie location
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "system.h"
#include "point.h"

typedef struct {
   Point position;
   uint32_t health;
} Player;

Player     players[2];

void player_set_won(void);

void player_set_lost(void);

bool player_has_won(void);

bool player_has_lost(void);

void player_init(Player players[], bool is_host);
void player_update(void *data);
Point player_get_position(uint8_t id);
uint8_t player_get_health(void);
void player_decrease_health(void);
void player_set_other_player_x(int8_t x_position);
void player_set_other_player_y(int8_t y_position);


#endif
