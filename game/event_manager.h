/** @file   event_manager.h
    @author Liam Diprose & Jeremy Craig
    @date   9 October 2016
    @brief  A counter based event manager.

*/

#ifndef MANAGER_H
#define MANAGER_H

#include "system.h"
#include "timer.h"

typedef void (* event_func_t)(void *data);

typedef struct event_struct
{
    event_func_t func;
    void *data;
    timer_tick_t period;    
    uint32_t counter;
} Event_t;

// Manages all events given
void event_manager (Event_t *events, uint8_t num_events);

#endif

