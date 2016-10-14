/** @file   event_manager.c
    @author Liam Diprose & Jeremy Craig
    @date   9 October 2016
    @brief  A counter based event manager.
*/
#include "system.h"
#include "event_manager.h"
#include "timer.h"

// - Event manager ----------------------------------------------------------
//  Runs each event when the individual event's time has runout
// ---------------------------------------------------------------------------
void event_manager (Event_t *events, uint8_t num_events){
    // Initilize timer
    timer_init ();
    register uint8_t current_event = 0;

    while (1){

        // loop through all events
        for (current_event = 0; current_event < num_events; current_event++ ){
            (events + current_event)->counter += 1;

            // if the individual events's counter has reached the specifed period
            if ((events + current_event)->counter > (events + current_event)->period){
                
                // reset counter
                (events + current_event)->counter = 0;

                // execute event's function
                (events + current_event)->func ((events +current_event)->data);
            }

        }
    }
}
