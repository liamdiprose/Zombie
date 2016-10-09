#include "system.h"
#include "level.h"
#include "point.h"


char level_data[LEVEL_HEIGHT][LEVEL_WIDTH];

void level_init(char data[][LEVEL_WIDTH]){
    /*int x; int y;
    for (x = 0; x < LEVEL_WIDTH; x++){
        for (y = 0; y < LEVEL_WIDTH; y++){
            
                if ((y%4) == 0 && (x%4) == 0){
                    data[y][x] = BACKGROUND_CHAR;
                } else {
                    data[y][x] = EMPTY_CHAR;
                }
                
            
        }
    }*/

}


char level_get_point(point pt) {
	return level_data[pt.y][pt.x];	
}

point level_get_opponent() {
	
}
