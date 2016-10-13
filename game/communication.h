/** @file   communication.h
    @author Liam Diprose & Jeremy Craig
    @date   13 October 2016

    @brief Routines for interfacing with the ir_uart
*/

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

// Get character from serail
char comm_getc(void);

// Add a message to queue to be sent over IR serial
void comm_mqueue_append(char message);

// If IR is ready, send the next message in the queue
void send_next_message(__unused__ void *data);

#endif
