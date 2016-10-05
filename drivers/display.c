/** @file   display.c
    @author M. P. Hayes, UCECE
    @date   23 August 2010
    @brief  Bit-mapped display driver.
*/
#include "system.h"
#include "ledmat.h"
#include "display.h"

#define PULSE_MAX 250
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


static uint8_t col = 0;
/** Update display (perform refreshing).  */
uint8_t display_update (void)
{
    ledmat_display_column (display[col], col);
    
    col++;
    if (col >= DISPLAY_WIDTH)
        col = 0;

    return col;
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

    uint8_t init_screen[DISPLAY_WIDTH][DISPLAY_HEIGHT] = {
        {0,1,0,20,0,1,0},
        {0,1,0,20,0,1,0},
        {0,1,0,20,0,1,0},
        {0,1,0,20,0,1,0},
        {0,1,0,20,0,1,0}
    };

    for (x = 0; x < DISPLAY_WIDTH; x++)
    {
        for (y = 0; y < DISPLAY_HEIGHT; y++)
        {
            //screen_display[x][y] = (x+y)%2;
            screen_display[x][y] = init_screen[x][y];
        }
    }

    ledmat_init ();
    display_clear ();
}

static uint8_t pwm_tick = 0;
static int current_col = 0;

void display_pulse(uint8_t screen_display[DISPLAY_WIDTH][DISPLAY_HEIGHT]){

}

void display_draw(uint8_t screen_display[DISPLAY_WIDTH][DISPLAY_HEIGHT]){
    //static int pixel_pulse = 0;
    //static bool is_fadein = true;
/*
    if (is_fadein){
        pixel_pulse++;
        if (pixel_pulse >= PULSE_MAX){
            is_fadein = true;
        }
    }else{
        pixel_pulse--;
        if (pixel_pulse <= 0){
            is_fadein = true;
        }
    }
  */  


    if (pwm_tick > 40){
        pwm_tick = 0;
    } 

    
    
    display_pixel_set ( current_col, 0, pwm_tick < screen_display[current_col][0]);
    display_pixel_set ( current_col, 1, pwm_tick < screen_display[current_col][1]);
    display_pixel_set ( current_col, 2, pwm_tick < screen_display[current_col][2]);
    display_pixel_set ( current_col, 3, pwm_tick < screen_display[current_col][3]);
    display_pixel_set ( current_col, 4, pwm_tick < screen_display[current_col][4]);
    display_pixel_set ( current_col, 5, pwm_tick < screen_display[current_col][5]);
    display_pixel_set ( current_col, 6, pwm_tick < screen_display[current_col][6]);
    
    pwm_tick = pwm_tick + 1;
    
    current_col = display_update ();
}