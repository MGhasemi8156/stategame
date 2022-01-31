#ifndef __HEADER_POTION_H__
#define __HEADER_POTION_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "map.h"
#include "soldier.h"

#define MAX_POTIONS 4


typedef struct Potion {
    double x;
    double y;
    int n;
    
    int type;
    int target_side;
    int life_time;
} Potion;

void add_potion(int* potions_n, Potion potions[], int lands_n, Land lands[]);

void apply_potions(SDL_Renderer* Renderer, int potions_n, Potion potions[],
                   int lands_n, Land lands[], int soldiers_n, Soldier *soldiers);

void remove_expired_potions(int* potions_n, Potion potions[]);
#endif 
