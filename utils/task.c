/** @file   task.c
    @author M. P. Hayes, UCECE
    @date   17 August 2011
    @brief  Simple task scheduler.
*/
#include "system.h"
#include "task.h"
#include "timer.h"


/** With 16-bit times the maximum value is 32768.  */
#define TASK_OVERRUN_MAX 32767


/** Schedule tasks
    @param tasks pointer to array of tasks (the highest priority
                 task comes first)
    @param num_tasks number of tasks to schedule
    @return this never returns.
*/
void task_schedule (task_t *tasks, uint8_t num_tasks, uint32_t num_ticks_per_update)
{
    timer_init ();

    uint32_t ticks = 0;
    uint8_t current_task = 0;

    while (1){

        for (current_task = 0; current_task < num_tasks; current_task++ ){
            if (ticks % (tasks + current_task)->period == 0){
                (tasks + current_task)->func ((tasks +current_task)->data);
            }
        }

        ticks++;
        if (ticks >= num_ticks_per_update){
            ticks = 0;
        }
    }
}
