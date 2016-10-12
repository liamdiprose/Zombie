#include "ir_uart.h"
#include "communication.h"
#include "pacer.h"
#include "point.h"
#include "player.h"

/*
typedef struct message_s Message;
struct message_s {
		char message;
		Message* next;
};
Message* queue_last;
Message* message_queue = NULL;
uint8_t mqueue_size = 0;
*/

char message_queue[MAX_MQUEUE_SIZE];
uint8_t head = 0;
uint8_t tail = -1;
int message_count = 0;

// TODO: change code
char comm_mqueue_pop() {
		char message = message_queue[head++];

		if (head == MAX_MQUEUE_SIZE) {
				head = 0;
		}
		message_count--;
		return message;
}

// TODO: change code
void comm_mqueue_append(char message) {
		if (message_count < MAX_MQUEUE_SIZE) {
				if (tail == MAX_MQUEUE_SIZE -1) {
						tail = -1;
				}

				message_queue[++tail] = message;
				message_count++;
		}
}



// Initiate IR for communication
void comm_init() {
	ir_uart_init();
}

/*
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
*/


void send_point(point pt) {

		comm_mqueue_append(pt.x & ~(1 << 7));
		comm_mqueue_append(pt.y | 1 << 7);
}


// Recevie one byte of a position (either x or y), based on the first bit of the register
void update_player_position(__unused__ void* data) {
		if (ir_uart_read_ready_p() ) {
				char recved = ir_uart_getc();
				if ((recved >> 7) & 1) {
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

