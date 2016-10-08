#include "system.h"
#include "display.h"
#include "level.h"
#include "point.h"

void level_init(char data[][LEVEL_WIDTH]){
    int x; int y;
    for (x = 0; x < LEVEL_WIDTH; x++){
        for (y = 0; y < LEVEL_WIDTH; y++){
            if ( (y == 0) || (x == 0) ){
                data[y][x] = WALL_CHAR;
            } else if ( (y == LEVEL_HEIGHT - 1) || (x == LEVEL_WIDTH - 1) ){
                data[y][x] = WALL_CHAR;
            } else {
                data[y][x] = EMPTY_CHAR;
            }
        }
    }
}