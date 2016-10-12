#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "point.h"

// Configure who is server and who is client
bool protocol_init();

// Search for a server <timeout> times
bool protocol_find_server(uint8_t timeout);

// Advertise server forever
void protocol_server_advertise();

char message_set_entity(char message, bool is_player);
char message_set_axis(char message, bool is_y);
bool message_is_player_entity(char message);
bool message_is_y_axis(char message);
char message_strip(char message);

void protocol_write_player(__unused__ void* data);
void protocol_read_player(__unused__ void* data);
#endif
