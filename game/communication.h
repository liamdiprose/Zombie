#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "point.h"

#define CLIENT_RESPONSE_TIMEOUT 250
#define SETUP_LISTEN_FOR_SERVER_TIME
#define PROTOCOL_SERVER 's'
#define PROTOCOL_CLIENT 'c'
#define PACER_RATE 500
#define NULL 0
#define MAX_MQUEUE_SIZE 50

// Initilise communication
void comm_init(void);

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
