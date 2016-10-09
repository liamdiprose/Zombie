#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "gametypes.h"
// Initilise communication
void communication_init(void);

bool comm_setup_protocol();

// Calculate checksum to identify incorrect transmission
uint8_t point_checksum(point pt);

// Attempt to send zombie, returns SEND_FAIL on failure
uint8_t point(point pt);

// Attempt to recevie zombie
Zombie* recv_zombie(void);

// Wait for serial interface to be ready to read
void wait_for_serial(uint8_t timeout);

#endif
