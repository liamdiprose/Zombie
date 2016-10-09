#include "system.h"
#include "navswitch.h"
#include "pio.h"
<<<<<<< HEAD
#include "level.h"
=======
>>>>>>> 80b432be3a54b6755150eed4a48d6ce5090959ac
#include "player.h"
#include "point.h"
#include "pacer.h"

void player_init(player players[]){
<<<<<<< HEAD
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
=======
     players[0].position.x = 3;
     players[0].position.y = 3;
     navswitch_init ();
}

void player_update(player *players)
{
    static bool init = 0;

    if (!init)
    {
        navswitch_init ();
        init = 1;
    }

    navswitch_update ();

    if (navswitch_push_event_p (NAVSWITCH_SOUTH) == true)
    {   
        players[0].position.y += 1;
    }

    if (navswitch_push_event_p (NAVSWITCH_EAST) == true)
    {   
        players[0].position.x += 1;
    }
    
    if (navswitch_push_event_p (NAVSWITCH_NORTH))
    {
        players[0].position.y -= 1;
    }
    
    if (navswitch_push_event_p (NAVSWITCH_WEST))
    {   
        players[0].position.x -= 1;
    }
    
>>>>>>> 80b432be3a54b6755150eed4a48d6ce5090959ac
    /*if (navswitch_push_event_p (NAVSWITCH_PUSH))
    {   
        players[0].position.x = 0;
    }*/
}