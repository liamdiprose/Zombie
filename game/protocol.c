#include "ir_uart.h"
#include "communication.h"
#include "pacer.h"
#include "point.h"
#include "protocol.h"
#include "player.h"
#include "level.h"

#define BIT_AXIS 7
#define BIT_UNIT 6
#define HANDLE_X 0
#define HANDLE_Y 1
#define HANDLE_PLAYER 0
#define HANDLE_ZOMBIE 1

#define MESSAGE_ENTITY_BIT 7
#define MESSAGE_AXIS_BIT 6
#define MESSAGE_BOARD_BIT 5

bool _is_host = false;
uint8_t last_zombie_y = 0;
uint8_t last_zombie_x = 0;

// Find out who is server and who is client
bool protocol_init()
{
    if (protocol_find_server(254)) {
        return false;           // Found server
    } else {
        protocol_server_advertise();
        _is_host = true;
        return true;
    }
}

// Find a server that is advertising a game
bool protocol_find_server(uint8_t timeout)
{
    uint8_t count = 0;
    bool server_found = false;
    pacer_init(PACER_RATE);
    while (count < timeout) {
        pacer_wait();
        if (ir_uart_read_ready_p()) {
            if (ir_uart_getc() == PROTOCOL_SERVER) {
                ir_uart_putc(PROTOCOL_CLIENT);
                return true;
            }
        }
        count++;
    }
    return false;
}

// Advertise a server forever
void protocol_server_advertise()
{
    pacer_init(500);
    while (1) {
        ir_uart_putc(PROTOCOL_SERVER);

        pacer_wait();
        if (ir_uart_read_ready_p()) {
            if (ir_uart_getc() == PROTOCOL_CLIENT) {
                return;
            }
        }
    }
}


// Zombies Update::
// 1. Send y coord
// 2. Send range of x coords
// 3. Send next y coord
//
//      0               0               0               0               0               0               0               0
//      Z/P             X/Y       |                                             VALUE 
// 0Z 1P        0X 1Y


char message_set_entity(char message, bool is_player)
{
    if (is_player) {
        return message | 1 << MESSAGE_ENTITY_BIT;
    } else {
        return message & ~(1 << MESSAGE_ENTITY_BIT);
    }
}

char message_set_axis(char message, bool is_y)
{
    if (is_y) {
        return message | 1 << MESSAGE_AXIS_BIT;
    } else {
        return message & ~(1 << MESSAGE_AXIS_BIT);
    }
}

char message_set_board(char message, bool is_host)
{
    if (is_host) {
        return message | 1 << MESSAGE_BOARD_BIT;
    } else {
        return message & ~(1 << MESSAGE_BOARD_BIT);
    }
}

// Return true if message contains player orinate, false if zombie
bool message_is_player_entity(char message)
{
    return (message >> MESSAGE_ENTITY_BIT) & 1;
}

// Return true if message contains Y ordinate, false if X
bool message_is_y_axis(char message)
{
    return (message >> MESSAGE_AXIS_BIT) & 1;
}

bool message_is_host(char message)
{
    return (message >> MESSAGE_BOARD_BIT) & 1;
}

char message_strip(char message)
{
    return message & ~(7 << 5);
}

void protocol_send_player(point pt)
{
    char message_x = pt.x;
    message_x = message_set_entity(message_x, true);
    message_x = message_set_axis(message_x, false);
    message_x = message_set_board(message_x, _is_host);
    comm_mqueue_append(message_x);

    char message_y = pt.y;
    message_y = message_set_entity(message_y, true);
    message_y = message_set_axis(message_y, true);
    message_y = message_set_board(message_y, _is_host);
    comm_mqueue_append(message_y);
}

void protocol_write_player(__unused__ void *data)
{
    protocol_send_player(players[0].position);
}

// Send new row for zombies to be added to
void protocol_write_zombie_row(uint8_t new_y)
{
    char message = new_y;
    message = message_set_entity(message, false);
    message = message_set_axis(message, true);
    message = message_set_board(message, _is_host);
    comm_mqueue_append(message);
}

// Send an x value following a y value
void protocol_write_zombie_col(uint8_t x_val)
{
    char message = x_val;
    message = message_set_entity(message, false);
    message = message_set_axis(message, false);
    message = message_set_board(message, _is_host);
    comm_mqueue_append(message);
}

void protocol_write_zombie(point zombie)
{
    if (zombie.y != last_zombie_y) {
        protocol_write_zombie_row(zombie.y);
        last_zombie_y = zombie.y;
    }
    protocol_write_zombie_col(zombie.x);
}

void protocol_read_zombie(char message)
{
    // Assuming the entity bit has already been checked for zombie, and not host
    if (_is_host) {
        return;
    }
    if (message_is_y_axis(message)) {
        last_zombie_y = message_strip(message);
        for (int i = 1; i < 4; i++) {
            level_set_point((point) {
                            i + last_zombie_x, last_zombie_y},
                            LEVEL_EMPTY);
        }
        last_zombie_x = 0;
    } else {
        uint8_t x_val = message_strip(message);
        for (; last_zombie_x < x_val; ++last_zombie_x) {
            level_set_point((point) {
                            last_zombie_x, last_zombie_y}
                            , LEVEL_EMPTY);
        }
        level_set_point((point) {
                        x_val, last_zombie_y}
                        , LEVEL_ZOMBIE);
    }
}


void protocol_read_player(char message)
{
    if (message_is_y_axis(message)) {
        players[1].position.y = message_strip(message);
    } else {
        players[1].position.x = message_strip(message);
    }
}

void protocol_read(__unused__ void *data)
{
    char message = comm_getc();
    if (message != 0) {
        if (message_is_host(message) ^ _is_host) {
            if (message_is_player_entity(message)) {
                protocol_read_player(message);
            } else {
                protocol_read_zombie(message);
            }
        }
    }
}
