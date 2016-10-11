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

// Send player position to client TODO: Zombie information
void update_client(__unused__ void* data);

void receive_server(__unused__ void* data);
#endif
