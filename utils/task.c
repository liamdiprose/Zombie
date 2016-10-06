#include "system.h"
#include "task.h"
#include "timer.h"

void task_schedule (task_t *tasks, uint8_t num_tasks)
{
    timer_init ();
    register uint8_t current_task = 0;

    while (1){
        for (current_task = 0; current_task < num_tasks; current_task++ ){
            (tasks + current_task)->counter += 1;

            if ((tasks + current_task)->counter > (tasks + current_task)->period){
                (tasks + current_task)->counter = 0;
                (tasks + current_task)->func ((tasks +current_task)->data);
            }

        }
    }
}
