#ifndef PLAYER_H
#define PLAYER_H

#include "system.h"
#include "point.h"
#include "player.h"

typedef struct {
   point position;
   uint8_t health;
} player;

void player_init(player players[]);
void player_update(player *players);

#endif