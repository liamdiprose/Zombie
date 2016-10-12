#include "system.h"
#include "level.h"
#include "point.h"
#include "player.h"

#define CLIENT_UPDATE_XPADDING 3   // Proximity of zombies to send to client
#define CLIENT_UPDATE_YPADDING 4

char level_data[LEVEL_HEIGHT][LEVEL_WIDTH];
uint8_t p1_updated_zombies = 0;


void level_init()
{
    for (uint8_t x = 0; x < LEVEL_WIDTH; x++){
		for (uint8_t y = 0; y < LEVEL_WIDTH; y++){
			level_data[y][x] = LEVEL_EMPTY;
		}
    } 
}

void level_set_zombie(uint8_t x, uint8_t y){
    static uint8_t previous_x = 0;
    static uint8_t previous_y = 0;
    if (y == 0){

    } else if (y != previous_y){
        for (uint8_t count = previous_x+1; count < LEVEL_WIDTH; count++){
            level_data[y][count] = LEVEL_EMPTY;
        }
        previous_y = y;      
        previous_x = 0;
    }

    if (previous_x == LEVEL_WIDTH){

    } else if (x > previous_x){
        for (uint8_t count = previous_x + 1; count < x; count++){
            level_data[previous_y][count] = LEVEL_EMPTY;
        }
        level_data[previous_y][x] = LEVEL_ZOMBIE;
        previous_x = x;
    } else {
        for (uint8_t count = previous_x+1; count < LEVEL_WIDTH; count++){
            level_data[y][count] = LEVEL_EMPTY;
        }
        previous_x = LEVEL_WIDTH;
    }
    
}

char level_get_point(point pt)
{
    return level_data[pt.y][pt.x];
}

void level_set_point(point pt, char givenChar)
{
    level_data[pt.y][pt.x] = givenChar;
}

point level_get_opponent()
{

}

void level_move(point start, int8_t dest_x, int8_t dest_y)
{
	level_data[start.y][start.x] = LEVEL_EMPTY;
    bool hit_player = false; 
    for (uint8_t player_id =0; player_id <= 2; player_id++){
        if (player_get_position(player_id).x == dest_x && player_get_position(player_id).y == dest_y){
            hit_player = true;
            if (player_id == 0){
                player_decrease_health();
            }
        }
    }
    if (hit_player){
        level_data[start.y][start.x] = LEVEL_ZOMBIE;
    } else if (dest_x > start.x || dest_y > start.y){
        level_data[dest_y][dest_x] = LEVEL_ZOMBIE_MOVED;
    } else {
        level_data[dest_y][dest_x] = LEVEL_ZOMBIE;
    }
	
}

// 
bool nav_try_move(point zombie_position, int8_t dx, int8_t dy)
{
    point canditate = {zombie_position.x + dx, zombie_position.y + dy};
    if (level_get_point(canditate) == LEVEL_EMPTY) {
        level_move(zombie_position, canditate.x, canditate.y);
        return true;
    } else {
        return false;
    }
}


// Move a single zombie towards the player. Do not move if cannot move closer in x or y direction.
void nav_move_zombie(point zombie_pos, point player_pos)
{
	bool attempted_sideways_move = false;
    bool has_moved = false;
	
	point difference = {zombie_pos.x - player_pos.x, zombie_pos.y - player_pos.y};
    if (abs(difference.x) > abs(difference.y)) {
		// Try move sideways first 
        if (difference.x > 0) {
			// left
           // has_moved = nav_try_move(zombie_pos, (point) {-1, 0});
            has_moved = nav_try_move(zombie_pos, -1, 0);
        } else {
			// right
            //has_moved = nav_try_move(zombie_pos, (point) {1, 0});
            has_moved = nav_try_move(zombie_pos, 1, 0);
        }
        attempted_sideways_move = true;
    }
    if (!has_moved) {
        // Try move zombie up or down, if above failed or difference.y is greater 
        if (difference.y > 0) {
			// down
//            has_moved = nav_try_move(zombie_pos, (point) {0, 1});
            has_moved = nav_try_move(zombie_pos, 0, -1);
        } else {
			// up
//            has_moved = nav_try_move(zombie_pos, (point) {0, -1});
            has_moved = nav_try_move(zombie_pos, 0, 1);
        }

    }
    if (!has_moved && !attempted_sideways_move) {
		// Move left or right if hasnt tried yet, and moving up/down failed
        if (difference.x > 0) {
			// left
           // has_moved = nav_try_move(zombie_pos, (point) {-1, 0});
            has_moved = nav_try_move(zombie_pos, -1, 0);
        } else {
			// right
            //has_moved = nav_try_move(zombie_pos, (point) {1, 0});
            has_moved = nav_try_move(zombie_pos, 1, 0);
        }
    }
}


// Update a group of zombies (TODO: group unsed for the time being, only if we need to 
// break up the execusion of this potienttally CPU-hog of a function
void nav_update_zombie_group(void* data)
{
    // TODO: Implementups
	player* players = data;
	point player_pos = players[0].position;
    for (int8_t row = 0; row < LEVEL_HEIGHT; row++) {
        for (int8_t col = 0; col < LEVEL_WIDTH; col++) {
			if (level_data[row][col] == LEVEL_ZOMBIE) {
                nav_move_zombie((point) {col, row}, player_pos);
			} else if (level_data[row][col] == LEVEL_ZOMBIE_MOVED){
                level_data[row][col] = LEVEL_ZOMBIE;
            }
//            if (level_get_point( (point) {col, row}) == LEVEL_ZOMBIE) {
//            }
        }
    }
}

void level_update_client() {
		point client_pos = players[1].position;
		uint8_t x_start = 0;
		uint8_t y_start = 0;
		
		uint8_t x_finish = LEVEL_WIDTH;
		uint8_t y_finish = LEVEL_HEIGHT;

		// Only send zombies in clients proximity
		if (client_pos.x > CLIENT_UPDATE_XPADDING) {
			x_start = client_pos.x - CLIENT_UPDATE_XPADDING;
		}
		if (client_pos.x < LEVEL_WIDTH - CLIENT_UPDATE_XPADDING) {
			x_finish = client_pos.x + CLIENT_UPDATE_XPADDING;
		}

		if (client_pos.y > CLIENT_UPDATE_YPADDING) {
			y_start = client_pos.y - CLIENT_UPDATE_YPADDING;
		}
		if (client_pos.y < LEVEL_WIDTH - CLIENT_UPDATE_YPADDING) {
			   y_finish = client_pos.y + CLIENT_UPDATE_YPADDING;	
		}
		
		uint8_t row;
		uint8_t col;

		for (col = y_start; col < y_finish; col++) {
				// TODO: Send Y level
				for (row = x_start; row < x_finish; row++) {
					// TODO: Send Zombie				
				}
		}
}
