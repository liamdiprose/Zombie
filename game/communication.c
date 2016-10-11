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
		ir_uart_putc(pt.x);
		ir_uart_putc(pt.y);
}

point recv_point() {
		point ret = {.x = 0, .y = 0};
		char received_char;

		received_char = ir_uart_getc();
		if (received_char >= 0 && received_char < 128) {
			   ret.x = received_char;	
		}
		received_char = ir_uart_getc();
		if (received_char >= 0 && received_char < 128) {
			   ret.y = received_char;	
		}
		return ret;
}

// Server only method for sending zombie locations
void update_client(__unused__ void* data) {
	//1. Send server's player's position
	//2. Send number of zombies to send
	//3. Send zombies (as locations)
	
	send_point(players[0].position);

}	

	
void receive_server(__unused__ void* data) {
		if (ir_uart_read_ready_p() ) {
				players[1].position = recv_point() ;
		}
}
