#ifndef PLAYER_H
#define PLAYER_H

#include "system.h"
#include "point.h"
#include "player.h"

typedef struct {
   point position;
<<<<<<< HEAD
   uint32_t health;
} player;

void player_init(player players[]);
void player_update(void *data);
uint32_t player_get_beat_rate(player players[]);
=======
   uint8_t health;
} player;

void player_init(player players[]);
void player_update(player *players);
>>>>>>> 80b432be3a54b6755150eed4a48d6ce5090959ac

#endif