#ifndef __HEADER_MAP_H__
#define __HEADER_MAP_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

typedef struct Land {
    char path[100];
    int x;
    int y;
    int width;
    int height;
    double angle;
    int has_barrack;
    int side;
    Sint16 barrack_x;
    Sint16 barrack_y;
    Sint16 barrack_r;
    int soldiers;
    int selected;
} Land;
 
int load_map(char file_path[100], int *lands_n, Land lands[20]);

void apply_map(SDL_Renderer* Renderer, int lands_n, Land lands[20]);

#endif