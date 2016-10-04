#include "system.h"
#include "led.h"
#include "pacer.h"
#include "display.h"
#include "task.h"


// included for proof of concept
 typedef struct state_struct
 {
     uint8_t led;
     uint8_t on;
 } state_t;

// included for proof of concept
 static void led_task (void *data)
 {
     static count = 0;
     count = (count + 1) % 5;
     if (count == 0){
     state_t *pstate = data;

     led_set (pstate->led, pstate->on);
     pstate->on = !pstate->on;
     }
 }


 int main (void)
 {
    // Initilizes the screen_display
     uint8_t screen_display[DISPLAY_WIDTH][DISPLAY_HEIGHT];
     display_init(screen_display);
     

     state_t led1 = {.led = LED1, .on = 0}; // included for proof of concept

     task_t tasks[] =
     {
         {.func = display_draw, .period = TASK_RATE /6000, .data = &screen_display}, // drawing a test pattern
         {.func = led_task, .period = TASK_RATE / 10, .data = &led1}, // included for proof of concept
     };

     system_init ();
     led_init ();
    
     task_schedule (tasks, 2);
     return 0;
 }


