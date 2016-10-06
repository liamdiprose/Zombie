#include "system.h"
#include "pio.h"
#include "led.h"

// included for proof of concept
 void heartbeat_task (__unused__ void *data)
 {
     static bool on = false;
     led_set (LED1, on);
     on = !on;
 }