#include "system.h"
#include "led.h"
#include "pacer.h"
#include "display.h"
#include "task.h"
#include "heartbeat.h"
#include "level.h"

 int main (void)
 {
    // Initilizes the screen_display
     level_init();
     display_init();
     system_init ();
     led_init ();

     task_t tasks[] =
     {
         {.func = display_draw, .period = 1, .data = 0}, // drawing a test pattern
         {.func = display_pulse, .period = 800, .data = 0}, // drawing a test pattern
         {.func = display_setmap, .period = 400, .data = level_get_display(0,0)}, // drawing a test pattern
         {.func = heartbeat_task, .period = 6400, .data = 0}, // included for proof of concept
     };

     
    
     task_schedule (tasks, 4);
     return 0;
 }


