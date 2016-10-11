#include "system.h"
#include "navswitch.h"
#include "pio.h"
#include "level.h"
#include "player.h"
#include "point.h"
#include "pacer.h"


void player_init(player players[]){
     players[0].position.x = 2;
     players[0].position.y = 2;

     players[0].health = 6400;

     navswitch_init ();
}

void player_update(void *data)
{
    player* players = data;
    navswitch_update ();

    if (navswitch_push_event_p (NAVSWITCH_SOUTH) == true){   
         if (players[0].position.y >= LEVEL_HEIGHT-2){
            players[0].position.y = LEVEL_HEIGHT-2;
        } else {
            players[0].position.y += 1;
        }
    }

    if (navswitch_push_event_p (NAVSWITCH_EAST) == true){   
        if (players[0].position.x >= LEVEL_WIDTH-2){
            players[0].position.x = LEVEL_WIDTH-2;
        } else {
            players[0].position.x += 1;
        }
        
    }
    
    if (navswitch_push_event_p (NAVSWITCH_NORTH)){
        if (players[0].position.y <= 1){
            players[0].position.y = 1;
        } else {
            players[0].position.y -= 1;
        }
    }
    
    if (navswitch_push_event_p (NAVSWITCH_WEST)){   
        if (players[0].position.x <= 1){
            players[0].position.x = 1;
        } else {
            players[0].position.x -= 1;
        }
    }
    
    uint32_t player_get_beat_rate(player players[]){
        return players[0].health; 
    }
}
