#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "point.h"

// Initilise communication
void comm_init(void);


// Configure who is server and who is client
bool protocol_init();

// Search for a server <timeout> times
bool protocol_find_server(uint8_t timeout);

// Advertise server forever
void protocol_server_advertise();

// Send a 2D point to IR
void send_point(point pt);

// Receive a point on IR
point recv_point();

// Receive either x or y position for the player and set it
void update_player_position(__unused__ void* data);

// If IR is ready, send the next message in the queue
void send_next_message(__unused__ void* data);


// Send player position to client TODO: Zombie information
void update_client(__unused__ void* data);

void receive_server(__unused__ void* data);
#endif
