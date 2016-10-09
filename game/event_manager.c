/** @file   event_manager.c
    @author Jeremy Craig
    @date   9 October 2016
    @brief  A counter based event manager.

*/
#include "system.h"
#include "event_manager.h"
#include "timer.h"

/** Schedule tasks
    @param tasks pointer to array of tasks (the highest priority
                 task comes first)
    @param num_tasks number of tasks to schedule
    @return this never returns.
*/
void event_manager (event_t *events, uint8_t num_events){
    timer_init ();
    register uint8_t current_event = 0;

    while (1){
        for (current_event = 0; current_event < num_events; current_event++ ){
            (events + current_event)->counter += 1;

            if ((events + current_event)->counter > (events + current_event)->period){
                (events + current_event)->counter = 0;
                (events + current_event)->func ((events +current_event)->data);
            }

        }
    }
}
