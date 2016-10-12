#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "point.h"

// Configure who is server and who is client
bool protocol_init();

// Search for a server <timeout> times
bool protocol_find_server(uint8_t timeout);

// Advertise server forever
void protocol_server_advertise();


#endif