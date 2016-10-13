/** @file   communication.h
    @author Liam Diprose & Jeremy Craig
    @date   13 October 2016

    @brief Routines for interfacing with the ir_uart
*/

#include "ir_uart.h"
#include "communication.h"
#include "pacer.h"
#include "point.h"
// #include "player.h"

char message_queue[MAX_MQUEUE_SIZE];
uint8_t head = 0;
uint8_t tail = -1;
uint8_t message_count = 0;


char last_sent_message = 0;

// TODO: change code
char comm_mqueue_pop(void)
{
    char message = message_queue[head++];

    if (head == MAX_MQUEUE_SIZE) {
        head = 0;
    }
    message_count--;
    return message;
}

// TODO: change code
void comm_mqueue_append(char message)
{
    if (message_count != MAX_MQUEUE_SIZE) {
        if (tail == MAX_MQUEUE_SIZE - 1) {
            tail = -1;
        }
        message_queue[++tail] = message;
        message_count++;
    }
}


bool comm_mqueue_empty_p(void)
{
    return message_count == 0;
}

// Initiate IR for communication
void comm_init(void)
{
    ir_uart_init();
}

char comm_getc(void)
{
    if (ir_uart_read_ready_p()) {
        char recved = ir_uart_getc();
        return recved;
    }
    return '\0';
}

//TASK Send the next message in queue iff ir is ready to write
void send_next_message(__unused__ void *data)
{
    if (ir_uart_write_ready_p()) {
        if (!comm_mqueue_empty_p()) {
            char message = comm_mqueue_pop();
            ir_uart_putc_nocheck(message);
        }
    }
}
