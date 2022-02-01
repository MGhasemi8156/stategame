#ifndef __HEADER_MAP_H__
#define __HEADER_MAP_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>


#define MAX_LANDS 20
#define MAX_SIDES 6

#define REBIRTH_RATE 60
#define IMPARTIAL_REBIRTH_RATE 120

#define BARRACK_R 25

#define MAX_SOLDIERS 50
#define IMPARTIAL_MAX_SOLDIERS 20

 
#define PIXELS_L 100 // pixels array w and h
#define PIXELS_F 3   // pixels factor
#define PIXELS_W 4   // pixels w and h


typedef struct Land {   
    // for rand Land
    int pixels[PIXELS_L][PIXELS_L];

    int has_barrack;
    int side;
    
    Sint16 barrack_x;
    Sint16 barrack_y;
    Sint16 barrack_r;
    
    int soldiers;
    int max_soldiers;
    int rebirth_rate;
    int rebirth_timer;

    int selected;
    
    int attack_queue;
    int is_attaking;
    
    // potions stuff4
    int unlimited_rebirth;
} Land;

void create_rand_map(int lands_n, Land lands[], int players);

void apply_rand_map(SDL_Renderer* Renderer, int lands_n, Land lands[], Land* selected_land_ptr);

int load_rand_map(char file_path[100], int *lands_n, Land lands[]);

Uint32 get_side_normal_color(int side);

#endif
