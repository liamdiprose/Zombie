#include "ir_uart.h"
#include "communication.h"
#include "pacer.h"
#include "point.h"
#include "protocol.h"
#include "player.h"
#include "level.h"

#define BIT_AXIS 7
#define BIT_UNIT 6
#define HANDLE_X 0
#define HANDLE_Y 1
#define HANDLE_PLAYER 0
#define HANDLE_ZOMBIE 1

#define MESSAGE_ENTITY_BIT 7
#define MESSAGE_AXIS_BIT 6
#define MESSAGE_BOARD_BIT 5

#define UPDATE_XPADDING 4
#define UPDATE_YPADDING 5

bool _is_host = false;
uint8_t previous_y = LEVEL_HEIGHT;
uint8_t previous_x = 0;
uint8_t x_min = 0;
uint8_t x_max = LEVEL_WIDTH;
uint8_t y_min = 0;
uint8_t y_max = LEVEL_HEIGHT;

// Find out who is server and who is client
bool protocol_init()
{
    if (protocol_find_server(254)) {
        return false;           // Found server
    } else {
        protocol_server_advertise();
        _is_host = true;
        return true;
    }
}

// Find a server that is advertising a game
bool protocol_find_server(uint8_t timeout)
{
    uint8_t count = 0;
    bool server_found = false;
    pacer_init(PACER_RATE);
    while (count < timeout) {
        pacer_wait();
        if (ir_uart_read_ready_p()) {
            if (ir_uart_getc() == PROTOCOL_SERVER) {
                ir_uart_putc(PROTOCOL_CLIENT);
                return true;
            }
        }
        count++;
    }
    return false;
}

// Advertise a server forever
void protocol_server_advertise()
{
    pacer_init(500);
    while (1) {
        ir_uart_putc(PROTOCOL_SERVER);

        pacer_wait();
        if (ir_uart_read_ready_p()) {
            if (ir_uart_getc() == PROTOCOL_CLIENT) {
                return;
            }
        }
    }
}


char message_set_entity(char message, bool is_player)
{
    if (is_player) {
        return message | 1 << MESSAGE_ENTITY_BIT;
    } else {
        return message & ~(1 << MESSAGE_ENTITY_BIT);
    }
}

char message_set_axis(char message, bool is_y)
{
    if (is_y) {
        return message | 1 << MESSAGE_AXIS_BIT;
    } else {
        return message & ~(1 << MESSAGE_AXIS_BIT);
    }
}

char message_set_board(char message, bool is_host)
{
    if (is_host) {
        return message | 1 << MESSAGE_BOARD_BIT;
    } else {
        return message & ~(1 << MESSAGE_BOARD_BIT);
    }
}

// Return true if message contains player orinate, false if zombie
bool message_is_player_entity(char message)
{
    return (message >> MESSAGE_ENTITY_BIT) & 1;
}

// Return true if message contains Y ordinate, false if X
bool message_is_y_axis(char message)
{
    return (message >> MESSAGE_AXIS_BIT) & 1;
}

bool message_is_host(char message)
{
    return (message >> MESSAGE_BOARD_BIT) & 1;
}

char message_strip(char message)
{
    return message & ~(7 << 5);
}

void protocol_send_player(point pt)
{
    char message_x = pt.x;
    message_x = message_set_entity(message_x, true);
    message_x = message_set_axis(message_x, false);
    message_x = message_set_board(message_x, _is_host);
    comm_mqueue_append(message_x);

    char message_y = pt.y;
    message_y = message_set_entity(message_y, true);
    message_y = message_set_axis(message_y, true);
    message_y = message_set_board(message_y, _is_host);
    comm_mqueue_append(message_y);
}

void protocol_write_player(__unused__ void *data)
{
    protocol_send_player(players[0].position);
}

// Send new row for zombies to be added to
void protocol_write_zombie_row(uint8_t new_y)
{
    char message = new_y;
    message = message_set_entity(message, false);
    message = message_set_axis(message, true);
    message = message_set_board(message, _is_host);
    comm_mqueue_append(message);
}

// Send an x value following a y value
void protocol_write_zombie_col(uint8_t x_val) {
		char message = x_val;
		message = message_set_entity(message, false);
		message = message_set_axis(message, false);
		message = message_set_board(message, _is_host);
		comm_mqueue_append(message);
}

void protocol_write_zombie(point zombie) {
		if (zombie.y != previous_y) {
				protocol_write_zombie_row(zombie.y);
				previous_y = zombie.y;
		}
		protocol_write_zombie_col(zombie.x);
}


void protocol_read_player(char message) {
		if (message_is_y_axis(message)) {
			//players[1].position.y = message_strip(message);
			player_set_other_player_y(message_strip(message));
		} else {
			//players[1].position.x = message_strip(message);
			player_set_other_player_x(message_strip(message));
		}
}

void protocol_read_zombie(char message)
{
    // Assuming the entity bit has already been checked for zombie, and not host
    if (_is_host) {
        return;
    }

	
	// If y is lower than previous y, then its a new update, so clear all the rows from y_min to received y
	// Set x_last to x_min (calculated)
	// For every received x point, 
	// 		Clear up to the point
	// 		Place the point
	// 		Save the next point to x_last
	// When recevied next y (row has finished)
	// 		Clear from x_last to x_max
	// 		Clear rows from y_last + 1 up to next y
	// 		Set x_last to x_min
	// 		Repeat <forevery x point> 
	// Back to top

	if (message_is_y_axis(message)) {
			// If lower than previous y, recalculate x/y_min/_max
			// Clear rows from y_min up to new_y
			
			uint8_t new_y = message_strip(message);
			
			// Clear the rest of the line	
			for (; previous_x < x_max; previous_x++) {
					level_set_point((point) {previous_y, previous_x}, LEVEL_EMPTY);
			}

			// Set the bounds of the new rectangle to clear board around the cleints position
			if (new_y < previous_y) {
					if (players[0].position.x > UPDATE_XPADDING) {
						x_min = players[0].position.x - UPDATE_XPADDING;
					} else {
						x_min = 0;
					}
					if (players[0].position.x < LEVEL_WIDTH - UPDATE_XPADDING) {
						x_max = players[0].position.x + UPDATE_XPADDING;
					} else {
						x_max = LEVEL_WIDTH;
					}
					if (players[0].position.y > UPDATE_YPADDING) { 
						y_min = players[1].position.y - UPDATE_YPADDING;
					} else {
						y_min = 0;
					}
					if (players[0].position.y < LEVEL_HEIGHT - UPDATE_YPADDING) {
						y_max = players[0].position.y + UPDATE_YPADDING;
					} else {
						y_max = LEVEL_HEIGHT;
					}
			}

			// Clear 
			for (; previous_y < new_y; previous_y++) {
					for (uint8_t col = x_min; col < x_max; col ++) {
							level_set_point((point) {col, previous_y}, LEVEL_EMPTY);
					}
			}
			previous_x = x_min;
			previous_y = new_y+1;
	}
	else {
			// Clear from x_min up to the received point
			uint8_t new_x = message_strip(message);

			for (; previous_x < new_x; previous_x++) {
					level_set_point((point) {previous_x, previous_y}, LEVEL_EMPTY);
			} 
			level_set_point((point) {new_x, previous_y}, LEVEL_ZOMBIE);
			if (players[0].position.x == new_x && players[0].position.y == previous_y){
				player_decrease_health();
			}
			previous_x = new_x + 1;
	}

}



void protocol_read(__unused__ void *data)
{
    char message = comm_getc();
    if (message != 0) {
        if (message_is_host(message) ^ _is_host) {
            if (message_is_player_entity(message)) {
                protocol_read_player(message);
            } else {
                protocol_read_zombie(message);
            }
        }
    }
}
