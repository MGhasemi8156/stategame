#ifndef __HEADER_SOLDIER_H__
#define __HEADER_SOLDIER_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "map.h"

#define VF 3 // velocity factor
#define TILL_BIRTH_FACTOR 7


typedef struct Soldier {
    int till_birth;
    int born;
    
    double x;
    double y;
    Sint16 r;
    double vx;
    double vy;
    
    int power;

    int side;
    Land* source;
    Land* destination;
    
} Soldier;

void add_soldiers(int *soldiers_n, int *max_soldiers, Soldier **soldiers_ptr,
                  Land* source, Land* destination);

void apply_soldiers(SDL_Renderer* Renderer, int soldiers_n, Soldier *soldiers); 

void collision_detection(int soldiers_n, Soldier *soldiers, int lands_n, Land lands[]);

void remove_zero_power_soldiers(int* soldiers_n, Soldier *soldiers);
#endif
