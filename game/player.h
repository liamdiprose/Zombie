#ifndef PLAYER_H
#define PLAYER_H

#include "system.h"
#include "point.h"

typedef struct {
   point position;
   uint32_t health;
} player;

player     players[2];

void player_init(player players[], bool is_host);
void player_update(void *data);
point player_get_position(uint8_t id);
uint8_t player_get_health(void);
void player_decrease_health(void);

#endif
