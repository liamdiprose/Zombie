#include "system.h"

#include "navswitch.h"
#include "led.h"

void heartbeat_init( void ){
    led_init();
}

// included for proof of concept
 void heartbeat_task (__unused__ void *data)
 {
     static bool on = false;
     static uint8_t state = 0;
     static int32_t count = 0;
     static int8_t ratio = 1; 
     ratio = player_get_health();
     count--;

     if (ratio == 0){
         on = false;
     }
     else if(state == 0 && count <= 0){
         on = true;
         count = 10 * ratio;
         state = 1;
     } else if (state == 1 && count <= 0){
         on = false;
         count = 16 * ratio;
         state = 2;
     } else if (state == 2 && count <= 0){
         on = true;
         count = 17 * ratio;
         state = 3;
     } else if (state == 3 && count <= 0){
         on = false;
         count = 58 * ratio;
         state = 0;
     }

     led_set (LED1, on);
     //on = !on; //turned off cause stupid bright in my eyes
 }