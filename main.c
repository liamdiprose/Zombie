#include "system.h"
#include "led.h"
#include "pacer.h"
#include "draw.h"
#include "event_manager.h"
#include "heartbeat.h"
#include "level.h"
#include "player.h"
#include "point.h"
#include "navswitch.h"

 int main (void)
 {
     //TODO: REORGANISE COMMUNICATION AND MANAGEMENT
     // ADD : game variables

     player     players[2];
     void*      players_ptr = &players;

     char       level_data[LEVEL_HEIGHT][LEVEL_WIDTH];
     void*      level_data_ptr = &level_data[0];
     // is_server or is_host
     // level_data
     // zombie positions

     // init
     // players init(players)
     // level_init(level_layout)
     // heartbeat_init(player[].health)

     // Initilizes the screen_display
     system_init ();
     display_init ();
     level_init (level_data);
     player_init (players);
     heartbeat_init ();

     event_t events[] =
     {
         
         // for host
         // read input from host and client
         // update zombies
         // update player
         {.func = player_update,            .period = 1000,     .data = players_ptr},
         // send data to client
         // draw game
         {.func = display_pulse,            .period = 800,      .data = 0}, // drawing a test pattern
         {.func = display_set_camera,       .period = 200,      .data = players_ptr},
         {.func = display_convert_level,    .period = 400,      .data = level_data_ptr},
         {.func = display_set_player,       .period = 200,      .data = players_ptr},
         {.func = display_draw,             .period = 1,        .data = 0}, // drawing a test pattern
         // for client
         // read input from client
         // read data from server
         // draw game
         // send data to host
         {.func = heartbeat_task,   .period = 6400,     .data = 0}, // included for proof of concept
     };
     

     event_manager (events, 7);
     return 0;
 }


