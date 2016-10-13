#include "system.h"
#include "navswitch.h"
#include "pio.h"
#include "level.h"
#include "player.h"
#include "point.h"
#include "pacer.h"
#include "protocol.h"
// TODO: Move send_point to protocol.c

bool has_won = false;
bool has_lost = false;

void player_set_won(void){
    has_won = true;
}

void player_set_lost(void){
    has_lost = true;
}

bool player_has_won(void){
    return has_won;
}

bool player_has_lost(void){
    return has_lost;
}

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
     players[0].health = 8;
     players[1].health = 8;

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

        if (level_data[players[0].position.y][players[0].position.x] == LEVEL_ZOMBIE){
            player_decrease_health();
        }
		//comm_mqueue_append(players[0].position.y | 1 << 7); // TODO: Repeated code 
    	//protocol_handle_ir_output(true, false, players[0].position.y);
	}

    if (navswitch_push_event_p (NAVSWITCH_EAST) == true){   
        if (players[0].position.x >= LEVEL_WIDTH-2){
            players[0].position.x = LEVEL_WIDTH-2;
        } else {
            players[0].position.x += 1;
        }
		//comm_mqueue_append(players[0].position.x & ~(1 << 7)); //TODO: Repeated code
    	//protocol_handle_ir_output(true, true, players[0].position.x);
        if (level_data[players[0].position.y][players[0].position.x] == LEVEL_ZOMBIE){
            player_decrease_health();
        }
    }
    
    if (navswitch_push_event_p (NAVSWITCH_NORTH)){
        if (players[0].position.y <= 1){
            players[0].position.y = 1;
        } else {
            players[0].position.y -= 1;
        }
		//comm_mqueue_append(players[0].position.y | 1 << 7); // TODO: Repeated code
    	//protocol_handle_ir_output(true, false, players[0].position.y);
        if (level_data[players[0].position.y][players[0].position.x] == LEVEL_ZOMBIE){
            player_decrease_health();
        }
    }
    
    if (navswitch_push_event_p (NAVSWITCH_WEST)){   
        if (players[0].position.x <= 1){
            players[0].position.x = 1;
        } else {
            players[0].position.x -= 1;
        }
		//comm_mqueue_append(players[0].position.x & ~(1 << 7)); // TODO: Repeated code
    	//protocol_handle_ir_output(true, true, players[0].position.x);
        if (level_data[players[0].position.y][players[0].position.x] == LEVEL_ZOMBIE){
            player_decrease_health();
        }
    }

    if (players[0].health <= 0){
        player_set_lost();
        protocol_send_player((point){LEVEL_WIDTH + 5, LEVEL_HEIGHT + 5});
    }
}

point player_get_position(uint8_t id){
    return players[id].position;
}

uint8_t player_get_health(void){
    return players[0].health;
}

void player_decrease_health(void){
    if (player_has_won()){
        players[0].health = 2;
        has_won = true;
        has_lost = false;
    }else if (player_has_lost()){
        players[0].health = 0;
        has_won = false;
        has_lost = true;
    }
    else if (players[0].health == 0){ 
        players[0].health = 0;
    } else {
        players[0].health -= 1;
    }
}

static bool got_y = false;
static bool got_x = false;

void player_set_other_player_x(int8_t x_position){
    if (x_position == LEVEL_WIDTH + 5){
        got_x = true;
    } else {  
        players[1].position.x = x_position;
    }
}

void player_set_other_player_y(int8_t y_position){
    

    if (got_x && got_y){
        player_set_won();
    } else if (y_position == LEVEL_HEIGHT + 5){
        got_y = true;
    } else {
        players[1].position.y = y_position;
    }
}

/*
void update_player_abs(__unused__ void* data) {
		protocol_send_player_x(players[0].position.x);
		protocol_send_player_y(players[0].position.y);
}
*/
