#include "ir_uart.h"
#include "gametypes.h"
#include "communication.h"
#include "pacer.h"

#define CLIENT_RESPONSE_TIMEOUT 250
#define SETUP_LISTEN_FOR_SERVER_TIME
#define SERVER_CHAR 's'
#define SETUP_LOOP_PACE 2
// Initiate IR for communication
void comm_init() {
		ir_uart_init();
}


bool comm_setup_protocol() {
		// 1. Listen for a server for 1 second roughly
		// 2. Become a server and broadcast stuff
		
		uint16_t listen_count = 0;
		pacer_init(SETUP_LOOP_PACE);

		for (listen_count = 0; listen_count < SETUP_LISTEN_FOR_SERVER_TIME; listen_count++) {
				if (ir_uart_read_ready_p()) {
						if (ir_uart_getc() == SERVER_CHAR) {
							return false;
						}

				}
		}
		// Failed to find server, becoming server
		// TODO: Change LED from blinking to solid

		while (1) {
			// 1. Send char
			// 2. Wait a for a bit
			// 3. If ready, read incoming, for 'c'
			// 4. If yes, return True, else, false				

			ir_uart_putc_nocheck(SERVER_CHAR);
			pacer_wait();
			if (ir_uart_read_ready_p() ) {
					if (ir_uart_getc() == PROTOCOL_CLIENT) {
							return true;
					}
			}
		}
}


// Wait for serial to be ready, or timeout
uint8_t wait_count = 0;
void wait_for_serial(uint8_t timeout) {
	while (!ir_uart_read_ready_p() || wait_count > timeout) {
		_delay_ms(10);
		wait_count++;
	}
}

// Calculate checksum for a zombie structure
uint8_t zombie_checksum(Zombie zombie) {
		return zombie.x + zombie.y;
}

// Send one zombie to IR uart
void send_zombie(Zombie zombie) {
		uint8_t recv_chksum = 0;
		uint8_t real_chksum = zombie_checksum(zombie)

		while (recv_chksum != real_chksum) {
				// Send Zombies x co-ordinate
				ir_uart_putc(zombie.x);
				// Send Zombies y co-ordinate 
				ir_uart_putc(zombie.y);
                wait_for_serial(CLIENT_RESPONSE_TIMEOUT);
				// Wait for response checksum
				if (ir_uart_read_ready_p()) {
					   recv_chksum = ir_uart_getc();
				}
		return;
}


Zombie* recv_zombie(void) {
        Zombie* new_zombie = NULL;
        uint8_t recv_chksum = 0;
        uint8_t real_chksum = 1;

        while (real_chksum != recv_checksum) {

                wait_for_serial(SERVER_WAIT_TIMEOUT);
                if (ir_uart_read_ready_p() ) {
                        new_zombie->x = ir_uart_getc();
                } else {
                   // Timeout occured
                    continue;
                }

                wait_for_serial(SERVER_WAIT_TIMEOUT);
                if (ir_uart_read_ready_p() ) {
                    new_zombie->y = ir_uart_getc();
                } else {
                        continue;
                }

                recv_chksum = zombie_checksum(*new_zombie);

                wait_for_serial(SERVER_WAIT_TIMEOUT);
                if (ir_uart_read_ready_p()) {
                    real_chksum = ir_uart_getc();
                } else {
                    continue;
                }
        }
        return new_zombie;
}


void send_zombies(Zombie* zombies, uint8_t num_zombies) {
	for (uint8_t i = 0; i < num_zombies; i++) {
			send_zombie(zombies[i]);
	}
}





