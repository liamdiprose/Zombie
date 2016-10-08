#include "system.h"
#include "navswitch.h"
#include "pio.h"
#include "player.h"
#include "point.h"
#include "pacer.h"

void player_init(player players[]){
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
    
    /*if (navswitch_push_event_p (NAVSWITCH_PUSH))
    {   
        players[0].position.x = 0;
    }*/
}