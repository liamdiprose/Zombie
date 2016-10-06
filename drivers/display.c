/** @file   display.c
    @author M. P. Hayes, UCECE
    @date   23 August 2010
    @brief  Bit-mapped display driver.
*/
#include "system.h"
#include "ledmat.h"
#include "display.h"


uint8_t screen_display[DISPLAY_WIDTH][DISPLAY_HEIGHT];
uint8_t draw_display[DISPLAY_WIDTH][DISPLAY_HEIGHT];
static uint8_t display[DISPLAY_WIDTH];
static uint8_t col = 0;
static uint8_t pwm_tick = 0;
static int current_pulse_value = 0;
static bool is_pulse_fade_in = true;

/** Set state of a display pixel.
    @param col pixel column
    @param row pixel row
    @param val pixel state.  */
void display_pixel_set (uint8_t col, uint8_t row, bool val)
{
    register uint8_t bitmask;
    register uint8_t pattern;

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
    register int col;

    for (col = 0; col < DISPLAY_WIDTH; col++)
        display[col] = 0;
}


/** Initialise display.   */
void display_init (void )
{
    register int x = 0;
    register int y = 0;
    
    for (x = 0; x < DISPLAY_WIDTH; x++)
    {
        for (y = 0; y < DISPLAY_HEIGHT; y++)
        {
            //screen_display[x][y] = (x+y)%2;
            screen_display[x][y] = 0;
            draw_display[x][y] = 0;
        }
    }

    ledmat_init ();
    display_clear ();
}

void display_pulse(__unused__ void *data){
    //static int current_pulse_value = 0;
    //static bool is_pulse_fade_in = true;
    register int x = 0;
    register int y = 0;

    if (is_pulse_fade_in){
        current_pulse_value++;
        if (current_pulse_value >= PULSE_MAX){
            is_pulse_fade_in = false;
        }
    }else{
        current_pulse_value--;
        if (current_pulse_value <= 1){
            is_pulse_fade_in= true;
        }
    }

    for (x = 0; x < DISPLAY_WIDTH; x++)
    {
        for (y = 0; y < DISPLAY_HEIGHT; y++)
        {
            //screen_display[x][y] = (x+y)%2;
            if (screen_display[x][y] > 40){
                draw_display[x][y] = current_pulse_value;
            } else {
                draw_display[x][y] = screen_display[x][y];
            }
        }
    }
}

static uint8_t current_col = 0;
static uint8_t frame_priority[41] = { 
    /*0*/  0,20,12,32,6,26,23,36,3,16,
    /*10*/ 29,9,24,4,34,14,40,21,1,37,
    /*20*/ 7,11,31,7,27,35,15,25,5,30,
    /*30*/ 10,22,39,2,19,33,13,28,8,18,
           38
    };
    

void display_setmap(__unused__ uint8_t data[][DISPLAY_HEIGHT]){
    register int x = 0;
    register int y = 0;
    for (x = 0; x < DISPLAY_WIDTH; x++)
    {
        for (y = 0; y < DISPLAY_HEIGHT; y++)
        {
            //screen_display[x][y] = (x+y)%2;
            screen_display[x][y] = data[x][y];
        }
    }
}

void display_draw(__unused__ void *data){

    if (pwm_tick > STEPS_OF_BRIGHTNESS){
        pwm_tick = 0;
    } 

    register uint8_t count;
    for (count = 0; count < DISPLAY_HEIGHT; count++){
    display_pixel_set ( current_col, count, frame_priority[pwm_tick] < draw_display[current_col][count]);
    }

    pwm_tick = pwm_tick + 1;
    
    current_col = display_update ();
}