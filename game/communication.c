#include "ir_uart.h"
#include "communication.h"
#include "pacer.h"
#include "point.h"
#include "player.h"

#define CLIENT_RESPONSE_TIMEOUT 250
#define SETUP_LISTEN_FOR_SERVER_TIME
#define PROTOCOL_SERVER 's'
#define PROTOCOL_CLIENT 'c'
#define PACER_RATE 500
#define NULL 0
#define MAX_MQUEUE_SIZE 10

typedef struct message_s Message;
struct message_s {
		char message;
		Message* next;
};
Message* queue_last;
Message* message_queue = NULL;
uint8_t mqueue_size = 0;

// Initiate IR for communication
void comm_init() {
	ir_uart_init();
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

void comm_mqueue_add(char message) {
	if (mqueue_size >= MAX_MQUEUE_SIZE) {
			return;
	}
	else {
			mqueue_size++;
			Message* new_message = {message, (Message*) NULL};
			if (queue_last == NULL || message_queue == NULL) {
				message_queue = new_message;
			} else {
				queue_last->next = new_message;
			}
			queue_last = new_message;
	}
}

char comm_mqueue_pop() {
		if (message_queue == NULL) {
				return '\0';
		} else {
				mqueue_size--;
				char message = message_queue->message;
				message_queue = message_queue->next;
				return message;
		}
}

// Find out who is server and who is client
bool protocol_init() {
		if (protocol_find_server(254)) {
				return false; // Found server
		} else {
				protocol_server_advertise();
				return true;
		}
}

void send_point(point pt) {

		comm_mqueue_add(pt.x & ~(1 << 7));
		comm_mqueue_add(pt.y | 1 << 7);
}


// Recevie one byte of a position (either x or y), based on the first bit of the register
void update_player_position(__unused__ void* data) {
		if (ir_uart_read_ready_p() ) {
				char recved = ir_uart_getc();
				if (recved >> 7) {
						// y
						players[1].position.y = recved & ~(1 << 7);
				} else {
						players[1].position.x = recved & ~(1 << 7);

				}
		}
}

// Send the next message in queue iff ir is ready to write
void send_next_message(__unused__ void* data) {
		if (ir_uart_write_ready_p() ) {
				char message = comm_mqueue_pop();
				if (message != '\0') {
					ir_uart_putc_nocheck(message);
				};
		}
}		

// Server only method for sending zombie locations
void update_client(__unused__ void* data) {
	//1. Send server's player's position
	//2. Send number of zombies to send
	//3. Send zombies (as locations)
	send_point(players[0].position);
}	


