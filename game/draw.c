/** @file   display.c
    @author M. P. Hayes, UCECE
    @date   23 August 2010
    @brief  Bit-mapped display driver.
    @modified
*/
#include "system.h"
#include "ledmat.h"
#include "level.h"
#include "draw.h"
#include "player.h"
#include "point.h"


static uint8_t display[DISPLAY_WIDTH];
static uint8_t col = 0;
static uint8_t pwm_tick = 0;
static int current_pulse_value = 0;
static bool is_pulse_fade_in = true;

uint8_t    screen_data[DISPLAY_HEIGHT][DISPLAY_WIDTH];
point      camera = {.x = 0, .y = 0};

uint8_t    screen_lose[DISPLAY_HEIGHT][DISPLAY_WIDTH] = {
    {0,0,0,0,0},
    {1,1,1,1,1},
    {1,0,1,0,1},
    {1,1,1,1,1},
    {0,1,1,1,0},
    {0,1,1,1,0},
    {0,0,0,0,0}
};

uint8_t    screen_win[DISPLAY_HEIGHT][DISPLAY_WIDTH] = {
    {0,0,0,0,0},
    {0,1,0,0,0},
    {0,1,0,0,0},
    {0,0,0,0,0},
    {1,0,0,0,1},
    {0,1,1,1,0},
    {0,0,0,0,0}
};

void display_set_camera(void *data)
{
    player* players = data;
    if (players[0].position.x - camera.x == 4){
        camera.x += 1;
    } else if(players[0].position.x - camera.x == 0){
        camera.x -= 1;
    }

    if (players[0].position.y - camera.y >= 5){
        if (players[0].position.y >= LEVEL_HEIGHT - 2){
            camera.y = LEVEL_HEIGHT - 7;
        } else {
            camera.y += 1;
        }
    } else if(players[0].position.y - camera.y <= 1){
        if (players[0].position.y <= 2){
            camera.y = 0;
        } else {
            camera.y -= 1;
        }
    }
}

void display_set_player(void *data)
{
    player* players = data;
    point p1_position;
    p1_position.x = players[0].position.x - camera.x;
    p1_position.y = players[0].position.y - camera.y;
    screen_data[p1_position.y][p1_position.x] = PLAYER_VALUE;

    point p2_position;
    p2_position.x = players[1].position.x - camera.x;
    p2_position.y = players[1].position.y - camera.y;
    if ( p2_position.x >= 0 && p2_position.x < DISPLAY_WIDTH ){
        if (  p2_position.y >= 0 && p2_position.y < DISPLAY_HEIGHT ){
            screen_data[p2_position.y][p2_position.x] = PLAYER_VALUE;
        }    
    }
}

void display_convert_level(__unused__ void* data){
    register int x;
    register int y;
    
    for (x = 0; x < DISPLAY_WIDTH; x++){
        for (y = 0; y < DISPLAY_HEIGHT; y++){
            screen_data[y][x] =EMPTY_VALUE;

            if (player_has_won()){
                if ( screen_win[y][x] == 1){
                    screen_data[y][x] = LEVEL_ZOMBIE;
                }
            } else if (player_has_lost()) {
                if ( screen_lose[y][x] == 1){
                    screen_data[y][x] = LEVEL_ZOMBIE;
                }
            } else {

            point current_position = (point){.x=camera.x + x, .y=camera.y + y};
            char current_char = level_get_point(current_position);
            
            if ((current_position.x%4) == 0 && (current_position.y%4) == 0 ){
                    screen_data[y][x] = BACKGROUND_VALUE;
            }

            switch(current_char){
                case LEVEL_PLAYER:
                    screen_data[y][x] = PLAYER_VALUE;
                break;
                case LEVEL_ZOMBIE:
                    screen_data[y][x] = current_pulse_value;
                break;
                
            }
            
            if (current_position.x == 0 || current_position.x == LEVEL_WIDTH-1){
                screen_data[y][x] = WALL_VALUE;
            }
            if (current_position.y == 0 || current_position.y == LEVEL_HEIGHT-1){
                screen_data[y][x] = WALL_VALUE;
            }
            }
            
        }    
    }
    

}

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
            screen_data[y][x] = 0;
        }
    }

    ledmat_init ();
    display_clear ();
}

void display_pulse(__unused__ void *data){
    if (is_pulse_fade_in){
        current_pulse_value++;
        if (current_pulse_value >= PULSE_MAX){
            is_pulse_fade_in = false;
        }
    }else{
        current_pulse_value--;
        if (current_pulse_value <= 0){
            is_pulse_fade_in= true;
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

void display_draw(__unused__ void *data){

    if (pwm_tick > STEPS_OF_BRIGHTNESS){
        pwm_tick = 0;
    } 

    register uint8_t count;
    for (count = 0; count < DISPLAY_HEIGHT; count++){
        display_pixel_set ( current_col, count, frame_priority[pwm_tick] < screen_data[count][current_col]);
    }

    pwm_tick = pwm_tick + 1;
    
    current_col = display_update ();
}
