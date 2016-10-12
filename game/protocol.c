#include "ir_uart.h"
#include "communication.h"
#include "pacer.h"
#include "point.h"
#include "protocol.h"
#include "player.h"

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

