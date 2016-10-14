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

// - Variables ----------------------------------------------------------
bool is_host = false;

// - Initilize game ----------------------------------------------------------
//
// ---------------------------------------------------------------------------
void initilization(void){
     system_init ();
     display_init ();
     level_init ();
     player_init (players, is_host);
     heartbeat_init ();
     comm_init();

     // Decide board's role
     is_host = protocol_init();
}

// - Host Events ----------------------------------------------------------
// Events to be run if board is set as the host
// --------------------------------------------------------------------------
void run_host(void){
    // Specify the original zombie positions
    level_set_zombie(0, 2);
    level_set_zombie(8, 0);

    level_set_zombie(0, 6);
    level_set_zombie(4, 0);
        
    level_set_zombie(0, 7);
    level_set_zombie(5, 0);

    // Specify the events to be run
	Event_t events[] = {
         {.func = player_update,            .period = 1000,     .data = players},
         {.func = level_update_client,	    .period = 5000,     .data = 0},
		 {.func = send_next_message, 		.period = 100, 		.data = 0},
         {.func = display_pulse,            .period = 800,      .data = 0}, // drawing a test pattern
         {.func = display_set_camera,       .period = 200,      .data = players},
         {.func = display_convert_level,    .period = 400,      .data = 0},
         {.func = display_set_player,       .period = 200,      .data = players},
         {.func = display_draw,             .period = 1,        .data = 0}, // drawing a test pattern
		 {.func = nav_update_zombie_group,	.period = 10000, 	.data = players},
         {.func = protocol_read,			.period = 100,	  	.data = 0}, 
         {.func = protocol_write_player,	.period = 1000,	  	.data = 0}, 
         {.func = level_add_zombie,		   .period = 100,     .data = 0},
         {.func = heartbeat_task,		   .period = 25,     .data = 0},
     };

     // Run given events in an event loop
     event_manager (events, 13);
}

// - Client Events ----------------------------------------------------------
// Events to be run if board is set as a client
// --------------------------------------------------------------------------
void run_client(void){
    // Specify the events to be run
    Event_t events[] = {
         {.func = player_update,            .period = 1000,     .data = players},
         {.func = send_next_message, 		.period = 100, 		.data = 0},
         {.func = display_pulse,            .period = 800,      .data = 0},
         {.func = display_set_camera,       .period = 200,      .data = players},
         {.func = display_convert_level,    .period = 400,      .data = 0},
         {.func = display_set_player,       .period = 200,      .data = players},
         {.func = display_draw,             .period = 1,        .data = 0},
         {.func = protocol_read,			.period = 100,	  	.data = 0}, 
         {.func = protocol_write_player,	.period = 1000,	  	.data = 0}, 
         {.func = heartbeat_task,   		.period = 25,     .data = 0},
    };
    
    // Run given events in an event loop
    event_manager (events, 10);
}

// Main program
int main (void)
{
     initilization();
     if(is_host){
         run_host();
     } else {
         run_client();
     }
     return 0;
 }


