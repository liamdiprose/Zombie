#include "system.h"
#include "navswitch.h"
#include "pio.h"
#include "level.h"
#include "player.h"
#include "point.h"
#include "pacer.h"
// TODO: Move send_point to protocol.c
#include "communication.h"

void player_init(player players[], bool is_host){
     if (is_host){
         players[0].position.x = 2;
         players[0].position.y = 3;
         
         players[1].position.x = 3;
         players[1].position.y = 2;
     
     } else {
         players[0].position.x = 3;
         players[0].position.y = 2;
         
         players[1].position.x = 2;
         players[1].position.y = 3;
     }
     players[0].health = 4;
     players[1].health = 4;

     

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
		comm_mqueue_append(players[0].position.y | 1 << 7); // TODO: Repeated code 
    }

    if (navswitch_push_event_p (NAVSWITCH_EAST) == true){   
        if (players[0].position.x >= LEVEL_WIDTH-2){
            players[0].position.x = LEVEL_WIDTH-2;
        } else {
            players[0].position.x += 1;
        }
		comm_mqueue_append(players[0].position.x & ~(1 << 7)); //TODO: Repeated code
        
    }
    
    if (navswitch_push_event_p (NAVSWITCH_NORTH)){
        if (players[0].position.y <= 1){
            players[0].position.y = 1;
        } else {
            players[0].position.y -= 1;
        }
		comm_mqueue_append(players[0].position.y | 1 << 7); // TODO: Repeated code
    }
    
    if (navswitch_push_event_p (NAVSWITCH_WEST)){   
        if (players[0].position.x <= 1){
            players[0].position.x = 1;
        } else {
            players[0].position.x -= 1;
        }
		comm_mqueue_append(players[0].position.x & ~(1 << 7)); // TODO: Repeated code
    }
    
    uint32_t player_get_beat_rate(player players[]){
        return players[0].health; 
    }
}

uint8_t player_get_health(void){
    return players[0].health;
}
