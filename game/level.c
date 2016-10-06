#include "system.h"
#include "display.h"
#include "level.h"

static char base_level_map[LEVEL_WIDTH + LEVEL_WALL_WIDTH][LEVEL_HEIGHT + LEVEL_WALL_WIDTH];
void level_init(void){
    register int x;
    register int y;

    for (x = 0; x < LEVEL_WIDTH + LEVEL_WALL_WIDTH; x++){
        for (y = 0; y < LEVEL_HEIGHT + LEVEL_WALL_WIDTH; y++){
            if ( (y == 0) || (x == 0) ){
                base_level_map[y][x] = WALL_CHAR;
            } else if ( (y == LEVEL_HEIGHT + LEVEL_WALL_WIDTH - 1) || (x == LEVEL_WIDTH + LEVEL_WALL_WIDTH - 1) ){
                base_level_map[y][x] = WALL_CHAR;
            } else {
                base_level_map[y][x] = EMPTY_CHAR;
            }
        }    
    }
}

static uint8_t result_display[DISPLAY_WIDTH][DISPLAY_HEIGHT];
uint8_t* level_get_display(uint8_t x_position, uint8_t y_position){
    register int x;
    register int y;
    
    for (x = 0; x < DISPLAY_WIDTH; x++){
        for (y = 0; y < DISPLAY_HEIGHT; y++){
            switch(base_level_map[x_position + x][ y_position + y]){
                case WALL_CHAR:
                    result_display[x][y] = WALL_VALUE;
                    break;
                case EMPTY_CHAR:
                    result_display[x][y] = EMPTY_VALUE;
                break;
                case BACKGROUND_CHAR:
                    result_display[x][y] = BACKGROUND_VALUE;
                break;
                case PLAYER_CHAR:
                    result_display[x][y] = PLAYER_VALUE;
                break;
                case ZOMBIE_CHAR:
                    result_display[x][y] = ZOMBIE_VALUE;
                break;
                
            }
        }    
    }

    return *result_display;
}