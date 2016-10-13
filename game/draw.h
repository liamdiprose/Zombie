/** @file   draw.h
    @author Liam Diprose & Jeremy Craig
    @date   13 October 2016

    @brief draw routines for zombie game
*/

#ifndef DRAW_H
#define DRAW_H

#include "system.h"
#include "level.h"
#include "point.h"
#include "player.h"

#define DISPLAY_WIDTH LEDMAT_COLS_NUM
#define DISPLAY_HEIGHT LEDMAT_ROWS_NUM

#define STEPS_OF_BRIGHTNESS 30

#define PULSE_MAX 15

/** Set state of a display pixel.
    @param col pixel column (0 left)
    @param row pixel row (0 top)
    @param val pixel state.  */

void display_set_camera(void *data);
void display_set_player(void *data);

void display_pixel_set (uint8_t col, uint8_t row, bool val);

void display_convert_level(__unused__ void* data);

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
