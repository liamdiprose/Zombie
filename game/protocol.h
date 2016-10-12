#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "point.h"

// Configure who is server and who is client
bool protocol_init();

// Search for a server <timeout> times
bool protocol_find_server(uint8_t timeout);

// Advertise server forever
void protocol_server_advertise();

void protocol_get_update();
void protocol_handle_ir_input(char given_message);
void protocol_handle_ir_output(bool is_player, bool is_x, int8_t position_value);

#endif