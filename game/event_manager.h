/** @file   event_manager.h
    @author Jeremy Craig
    @date   9 October 2016
    @brief  A counter based event manager.

*/

#ifndef MANAGER_H
#define MANAGER_H

#include "system.h"
#include "timer.h"

/** Task function prototype.  */
typedef void (* event_func_t)(void *data);


/** Task structure.  */
typedef struct event_struct
{
    /** Function to call.  */
    event_func_t func;
    /** Generic pointer to pass to function.  */
    void *data;
    /** How many ticks between scheduling events.  */
    timer_tick_t period;    

    uint32_t counter;
} event_t;


/** Schedule tasks
    @param tasks pointer to array of tasks (the highest priority
                 task comes first)
    @param num_tasks number of tasks to schedule
    @return this never returns.
*/
void event_manager (event_t *events, uint8_t num_events);

#endif

