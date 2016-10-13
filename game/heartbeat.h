/** @file   heartbeat.h
    @author Liam Diprose & Jeremy Craig
    @date   9 October 2016
    @brief  changes the led in a way that represents a heartbeat.
*/

#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include "system.h"

// Initilises heartbeat
void heartbeat_init ( void );

// updates the led based on the amount of times called
void heartbeat_task ( void *data );


#endif
