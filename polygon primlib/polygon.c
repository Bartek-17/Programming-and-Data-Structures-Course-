#include "primlib.h"
#include <stdlib.h>	
#include <unistd.h>

#define VERTICES 5
#define ANGLE_CHANGE 0.03
#define PI 3.14
#define SCALE_CHANGE 0.01
#define MIN_SIZE 0.4
#define MAX_SIZE 1.8

void drawPolygon(float angle, float scale) {
    int vertex;
    for (vertex = 1; vertex <= VERTICES; vertex++) {
        int x1 = gfx_screenWidth() / 2 + scale * cos(angle + vertex * 2 * PI / VERTICES) * gfx_screenWidth() / 4;
        int y1 = gfx_screenHeight() / 2 + scale * sin(angle + vertex * 2 * PI / VERTICES) * gfx_screenHeight() / 4;
        int x2 = gfx_screenWidth() / 2 + scale * cos(angle + (vertex + 1) * 2 * PI / VERTICES) * gfx_screenWidth() / 4;
        int y2 = gfx_screenHeight() / 2 + scale * sin(angle + (vertex + 1) * 2 * PI / VERTICES) * gfx_screenHeight() / 4;
        gfx_line(x1, y1, x2, y2, RED);
    }
}

int main(int argc, char* argv[]) {
    if (gfx_init()) {
        exit(3);
    }

    float rotationAngle = 0.0;
    float scale = 1.0;
    float scaleDirection = 1.0;


    while (gfx_isKeyDown(SDLK_e) == 0) {
        gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
        drawPolygon(rotationAngle, scale);
        gfx_updateScreen();


        rotationAngle += ANGLE_CHANGE;
        scale += SCALE_CHANGE * scaleDirection;

        if (rotationAngle >= 2*PI){
            rotationAngle -= 2*PI;
        }

        SDL_Delay(15);

        if (scale <= MIN_SIZE || scale >= MAX_SIZE) {
            // scaling direction change
            scaleDirection *= -1;
            
        }
    }
    return 0;
}
