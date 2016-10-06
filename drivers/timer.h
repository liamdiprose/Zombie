/** @file   timer.h
    @author M. P. Hayes, UCECE
    @date   18 August 2011
    @brief  Timer support.

    @defgroup timer Timer module

    This module provides simple timer support by abstracting a
    hardware timer.

    The rate that the timer is incremented is specified by the
    macro TIMER_CLOCK_DIVISOR in the header file system.h  This
    must be a value supported by the timer clock prescaler.

    The rate that the timer is incremented can be found using the
    macro TIMER_RATE.

    The timer is free-running and will increment TIMER_RATE times
    per second.  When the timer reaches 65535 on the next increment
    it rolls over to 0.
*/
#ifndef TIMER_H
#define TIMER_H

#include "system.h"


#ifndef TIMER_CLOCK_DIVISOR
#define TIMER_CLOCK_DIVISOR 1
#endif

/** Rate in Hz that the timer is incremented.  */
#define TIMER_RATE (F_CPU / TIMER_CLOCK_DIVISOR)


/** The maximum overrun (in ticks).  */
#define TIMER_OVERRUN_MAX 1000


/** The maximum delay (in ticks).  */
#define TIMER_DELAY_MAX (65536u - (TIMER_OVERRUN_MAX ))


/** Define timer ticks.  */
typedef uint16_t timer_tick_t;


/** Get current time:
    @return current time in ticks.  */
timer_tick_t timer_get (void);


/** Wait until specified time:
    @param when time to sleep until
    @return current time.  */
timer_tick_t timer_wait_until (timer_tick_t when);


/** Wait for specified period:
    @param period how long to wait
    @return current time.  */
timer_tick_t timer_wait (timer_tick_t period);


/** Initialise timer.  */
void timer_init (void);

#endif /* TIMER_H  */
