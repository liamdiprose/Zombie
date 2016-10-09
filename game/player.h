#ifndef PLAYER_H
#define PLAYER_H

#include "system.h"
#include "point.h"
#include "player.h"

typedef struct {
   point position;
   uint32_t health;
} player;

player     players[2];

void player_init(player players[]);
void player_update(void *data);
uint32_t player_get_beat_rate(player players[]);

#endif