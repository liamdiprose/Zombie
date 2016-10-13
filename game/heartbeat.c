/** @file   heartbeat.c
    @author Liam Diprose & Jeremy Craig
    @date   13 October 2016

    @brief updates the rate of led pulsing based on player health
*/

#include "system.h"
/** @file   heartbeat.c
    @author Liam Diprose & Jeremy Craig
    @date   9 October 2016
    @brief  changes the led in a way that represents a heartbeat.
*/
#include "navswitch.h"
#include "led.h"
#include "player.h"

// Initilises heartbeat
void heartbeat_init( void ){
    led_init();
}

// updates the led based on the amount of times called
 void heartbeat_task (__unused__ void *data)
 {
     static bool on = false;
     static uint8_t state = 0;
     static int32_t count = 0;
     static int8_t ratio = 1; 
     ratio = player_get_health();

     //decreases the count to zero
     count--;

     // if when the player health is zero, turn led off
     // else based on the current state toggle the led and increase timer to be decremented again
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

     // set the led to be on/off
     led_set (LED1, on);
 }
