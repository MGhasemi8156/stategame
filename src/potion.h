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
#define RAND_CREATION_RATE 1500


#define POTION_SIDES 6
#define POTION_R 20

// stop enemy potion. type: 1
#define STOP_ENEMY_DURATION 600

// increase velocity potion. type: 2
#define INCREASE_VELOCITY_FACTOR 3
#define INCREASE_VELOCITY_DURATION 300

// unlimited rebirth potion. type: 3
#define UNLIMITED_REBIRTH_DURATION 300

// increase rebirth rate potion. type: 4
#define INCREASE_REBIRTH_RATE_VALUE 30
#define INCREASE_REBIRTH_RATE_DURATION 300


typedef struct Potion {
    double x;
    double y;
    
    int type;
    int target_side;
    int till_end;
} Potion;


// include header in each other
typedef struct Soldier Soldier;

void add_potion(int* potions_n, Potion potions[], int lands_n, Land lands[]);

void apply_potions(SDL_Renderer* Renderer, int potions_n, Potion potions[],
                   int lands_n, Land lands[], int soldiers_n, Soldier *soldiers);

void remove_expired_potions(int* potions_n, Potion potions[]);
#endif 
