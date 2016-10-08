/** @file   display.h
    @author M. P. Hayes, UCECE
    @date   23 August 2010
    @brief  Bit-mapped display driver.

    @defgroup display Bit-mapped display driver
       
    This module implements a simple display frame-buffer.  It abstracts the
    multiplexing of a LED matrix display.
*/
#ifndef DISPLAY_H
#define DISPLAY_H

#include "system.h"
#include "level.h"
#include "point.h"
#include "player.h"

#define DISPLAY_WIDTH LEDMAT_COLS_NUM
#define DISPLAY_HEIGHT LEDMAT_ROWS_NUM

#define STEPS_OF_BRIGHTNESS 40

#define PULSE_MAX 15

/** Set state of a display pixel.
    @param col pixel column (0 left)
    @param row pixel row (0 top)
    @param val pixel state.  */

void display_set_camera(point position);
void display_set_player(player *players);
void display_pixel_set (uint8_t col, uint8_t row, bool val);

void display_convert_level(char level_data[][LEVEL_WIDTH]);

/** Update display (perform refreshing).  */
uint8_t display_update (void);

/** Clear display.   */
void display_clear (void);

/** Initialise display.   */
void display_init (void);

/** Update display.   */
void display_pulse(void *data);

/** Draw display.   */
void display_draw(void *data);

#endif
