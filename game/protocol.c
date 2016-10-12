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




// Find out who is server and who is client
bool protocol_init() {
		if (protocol_find_server(254)) {
				return false; // Found server
		} else {
				protocol_server_advertise();
				return true;
		}
}

// Find a server that is advertising a game
bool protocol_find_server(uint8_t timeout) {
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
			count ++ ;
		}
		return false;
}

// Advertise a server forever
void protocol_server_advertise() {
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

void protocol_get_update(){
    char received_char = comm_getc();
    if (received_char != '\0'){
        //if ((received_char >> 7) & 1) {
        //   // y
        //    players[1].position.y = received_char & ~(1 << 7) & ~(1 << 6);
        //} else {
        //    players[1].position.x = received_char & ~(1 << 7) & ~(1 << 6);
        //}
        protocol_handle_ir_input(received_char);
    }
}

void protocol_send_player_x(int8_t value){
    protocol_handle_ir_output(true, true, value);
}

void protocol_send_zombie(point value){
    static int8_t previous_y = 0;
    if (value.y != previous_y){
        protocol_send_zombie_y(value.y);
        previous_y = value.y;
    } 
    protocol_send_zombie_x(value.x);
}

void protocol_send_player_y(int8_t value){
    protocol_handle_ir_output(true, false, value);
}

void protocol_send_zombie_x(int8_t value){
    protocol_handle_ir_output(false, true, value);
}

void protocol_send_zombie_y(int8_t value){
    protocol_handle_ir_output(false, false, value);
}


void protocol_handle_ir_input(char given_message){

    int8_t stripped_message = (int8_t)(given_message & ~(1 << BIT_UNIT) & ~(1 << BIT_AXIS));
    //if ((received_char >> 7) & 1) {
        //   // y
        //    players[1].position.y = received_char & ~(1 << 7) & ~(1 << 6);
        //} else {
        //    players[1].position.x = received_char & ~(1 << 7) & ~(1 << 6);
        //}

    // is zombie
    if ((given_message >> BIT_UNIT) & HANDLE_ZOMBIE) {
        if (stripped_message != 0){
            if ((given_message >> BIT_AXIS) & HANDLE_Y) {
                level_set_zombie(0, stripped_message);
            } else {
                level_set_zombie(stripped_message, 0);
            }
        }
    // is player   
    } else {
        if (stripped_message != 0){
            if ((given_message >> BIT_AXIS) & HANDLE_Y) {
                players[1].position.y = stripped_message;
            } else {
                players[1].position.x = stripped_message;
            } 
        }
    }
}


void protocol_handle_ir_output(bool is_player, bool is_x, int8_t position_value){
    char to_send = (char)position_value;
    if (!is_player){
        to_send = to_send | HANDLE_ZOMBIE << BIT_UNIT;
    }
    if (!is_x){
        to_send = to_send | HANDLE_Y << BIT_AXIS;
    }
    comm_mqueue_append(to_send);

}

