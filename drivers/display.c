/** @file   display.c
    @author M. P. Hayes, UCECE
    @date   23 August 2010
    @brief  Bit-mapped display driver.
*/
#include "system.h"
#include "ledmat.h"
#include "display.h"

/** The state of the display (frame buffer).  */
static uint8_t display[DISPLAY_WIDTH];


/** Set state of a display pixel.
    @param col pixel column
    @param row pixel row
    @param val pixel state.  */
void display_pixel_set (uint8_t col, uint8_t row, bool val)
{
    uint8_t bitmask;
    uint8_t pattern;

    if (col >= DISPLAY_WIDTH || row >= DISPLAY_HEIGHT)
        return;

    bitmask = BIT (row);
    pattern = display[col] & ~bitmask;

    if (val)
        pattern |= bitmask;

    display[col] = pattern;
}


/** Get state of a display pixel.
    @param col pixel column
    @param row pixel row
    @return pixel state or zero if outside display.  */
bool display_pixel_get (uint8_t col, uint8_t row)
{
    uint8_t bitmask;

    if (col >= DISPLAY_WIDTH || row >= DISPLAY_HEIGHT)
        return 0;

    bitmask = BIT (row);

    return (display[col] & bitmask) != 0;
}


/** Update display (perform refreshing).  */
void display_update (void)
{
    static uint8_t col = 0;

    ledmat_display_column (display[col], col);
    
    col++;
    if (col >= DISPLAY_WIDTH)
        col = 0;
}


/** Clear display.   */
void display_clear (void)
{
    int col;

    for (col = 0; col < DISPLAY_WIDTH; col++)
        display[col] = 0;
}


/** Initialise display.   */
void display_init (uint8_t screen_display[DISPLAY_WIDTH][DISPLAY_HEIGHT])
{
    int x = 0;
    int y = 0;

    for (x = 0; x < DISPLAY_WIDTH; x++)
    {
        for (y = 0; y < DISPLAY_HEIGHT; y++)
        {
            screen_display[x][y] = (x+y)%2;
        }
    }

    ledmat_init ();
    display_clear ();
}

void display_draw(uint8_t screen_display[DISPLAY_WIDTH][DISPLAY_HEIGHT]){
    display_update ();
    static uint8_t pwm_tick = 0;
    pwm_tick++;
    int x = 0;
    int y = 0;
    for (x = 0; x < DISPLAY_WIDTH; x++)
    {
        for (y = 0; y < DISPLAY_HEIGHT; y++)
        {
            if (pwm_tick < 100){
                display_pixel_set ( x, y, screen_display[x][y] == 0);
            } else {
                display_pixel_set ( x, y, screen_display[x][y] == 1);
            }
        }
    }
    pwm_tick = pwm_tick % 200;
}