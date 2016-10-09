#include "system.h"
#include "pio.h"
#include "navswitch.h"
#include "led.h"

void heartbeat_init( void ){
    led_init();
}

// included for proof of concept
 void heartbeat_task (__unused__ void *data)
 {
     static bool on = false;
     led_set (LED1, on);
     on = !on; //turned off cause stupid bright in my eyes
 }