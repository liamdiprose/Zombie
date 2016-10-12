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
#include "communication.h"
#include "protocol.h"

bool is_host = false;

void initilization(void){
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
     level_init ();
     player_init (players, is_host);
     heartbeat_init ();
     comm_init();

     is_host = protocol_init();
}


void run_host(void){
    	
		
		event_t events[] =

     {
         
         // for host
         // read input from host and client
         // update zombies
         // update player
         {.func = player_update,            .period = 1000,     .data = players},
         // send data to client
		 // TODO: Make this part of the player_update function
		 {.func = send_next_message, 		.period = 500, 		.data = 0},
         // draw game
         {.func = display_pulse,            .period = 800,      .data = 0}, // drawing a test pattern
         {.func = display_set_camera,       .period = 200,      .data = players},
         {.func = display_convert_level,    .period = 400,      .data = 0},
         {.func = display_set_player,       .period = 200,      .data = players},
         {.func = display_draw,             .period = 1,        .data = 0}, // drawing a test pattern
		 {.func = nav_update_zombie_group,	.period = 10000, 	.data = players}, 
         // for client
         // read input from client
         // read data from server
         // draw game
         // send data to host
         {.func = heartbeat_task,   .period = 100,     .data = 0}, // included for proof of concept
     };
     

     event_manager (events, 9);
}

void run_client(void){
    event_t events[] =
     {
         
         // for host
         // read input from host and client
         // update zombies
         // update player
         {.func = player_update,            .period = 1000,     .data = players},
         {.func = send_next_message, 		.period = 500, 		.data = 0},
         // send data to client
         // draw game
         {.func = display_pulse,            .period = 800,      .data = 0}, // drawing a test pattern
         {.func = display_set_camera,       .period = 200,      .data = players},
         {.func = display_convert_level,    .period = 400,      .data = 0},
         {.func = display_set_player,       .period = 200,      .data = players},
         {.func = display_draw,             .period = 1,        .data = 0}, // drawing a test pattern
         {.func = protocol_get_update,		.period = 100,	  	.data = 0}, 
         // for client
         // read input from client
         // read data from server
         // draw game
         // send data to host
         {.func = heartbeat_task,   .period = 10,     .data = 0}, // included for proof of concept
     };
     

     event_manager (events, 8);
}


int main (void)
{
     initilization();
     if(is_host){
         run_host();
     }else{
         run_client();
     }
     return 0;
 }


