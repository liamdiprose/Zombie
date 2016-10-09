#ifndef COMMUNICATION_H
#define COMMUNICATION_H

// Initilise communication
void communication_init(void);

// Calculate checksum to identify incorrect transmission
uint8_t zombie_checksum(Zombie zombie);

// Attempt to send zombie, returns SEND_FAIL on failure
uint8_t send_zombie(Zombie zombie);

// Attempt to recevie zombie
Zombie* recv_zombie(void);

// Wait for serial interface to be ready to read
void wait_for_serial(uint8_t timeout);

#endif
