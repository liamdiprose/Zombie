#include "system.h"
#include "led.h"
#include "pacer.h"
#include "display.h"
#include "task.h"
#include "heartbeat.h"
#include "level.h"
#include "player.h"
#include "point.h"
#include "navswitch.h"


player     players[2];
char       level_data[LEVEL_HEIGHT][LEVEL_WIDTH];

 int main (void)
 {
     //TODO: REORGANISE COMMUNICATION AND MANAGEMENT
     // ADD : game variables
     
     
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

     task_t tasks[] =
     {
         
         // for host
         // read input from host and client
         // update zombies
         // update player
         {.func = player_update,         .period = 100,        .data = players},
         // send data to client
         // draw game
         {.func = display_convert_level,     .period = 400,        .data = level_data},
         {.func = display_set_player,        .period = 400,        .data = players},

         // for client
         // read input from client
         // read data from server
         // draw game
         // send data to host


         {.func = display_draw,     .period = 1,        .data = 0}, // drawing a test pattern
         {.func = display_pulse,    .period = 800,      .data = 0}, // drawing a test pattern
         {.func = heartbeat_task,   .period = 6400,     .data = 0}, // included for proof of concept
     };

     
    
     task_schedule (tasks, 6);
     return 0;
 }


