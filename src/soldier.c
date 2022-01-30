#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "map.h"
#include "soldier.h"

void add_soldiers(int *soldiers_n, int *max_soldiers, Soldier **soldiers_ptr,
                  Land* source, Land* destination) {
    // extend the memory if needed
    if (*soldiers_n + source->soldiers > *max_soldiers) {
        *soldiers_ptr = realloc(*soldiers_ptr, sizeof(Soldier) * (*max_soldiers + 500));
        *max_soldiers += 500;
    }
    
    // TODO: decrease array size
    
    // add soldiers
    for (int i = 0; i < source->soldiers; i++) {
        Soldier temp;
        
        temp.till_birth = i * 7;
        temp.born = 0;
        
        temp.x = (double)source->barrack_x;
        temp.y = (double)source->barrack_y;
        temp.r = 5; // TODO change
        
        double l = sqrt(pow((destination->barrack_x - temp.x), 2) +
                        pow((destination->barrack_y - temp.y), 2));
        // TODO add fraction
        temp.vx = ((double)destination->barrack_x - (double)temp.x)/l * 3;
        temp.vy = ((double)destination->barrack_y - (double)temp.y)/l * 3;
        
        temp.power = 1;
    
        temp.side = source->side;
        temp.source = source;
        temp.destination = destination;
        
        (*soldiers_ptr)[*soldiers_n + i] = temp;
    }
    
    *soldiers_n += source->soldiers;
    
    source->soldiers = 0;
}

void apply_soldiers(SDL_Renderer* Renderer, int *soldiers_n, Soldier *soldiers, int lands_n, Land lands[]) {
    for (int i = 0; i < *soldiers_n; i++) {
        if (!soldiers[i].born) { // birth check
            soldiers[i].till_birth -= 1;
            if (soldiers[i].till_birth <= 0) soldiers[i].born = 1;
        }
        else {
            // move
            soldiers[i].x += soldiers[i].vx;
            soldiers[i].y += soldiers[i].vy;
            // draw
            filledCircleColor(Renderer, (Sint16)soldiers[i].x, (Sint16)soldiers[i].y, soldiers[i].r, 0xffff0000);
            
        }
    }

    // collision detection
    for (int i = 0; i < *soldiers_n; i++) {
        // soldiers collisions
        for (int j = i + 1; j < *soldiers_n; j++) { 
            if (soldiers[i].side != soldiers[j].side && soldiers[i].power > 0 && soldiers[j].power > 0) {
                double d = sqrt(pow(soldiers[i].x - soldiers[j].y, 2) +
                                pow(soldiers[i].y - soldiers[j].y, 2));
                if (d < 8) {
                    int temp = soldiers[i].power;
                    soldiers[i].power -= soldiers[j].power;
                    soldiers[j].power -= temp;
                }
            }
        }
        // lands collisions
        for (int j = 0; j < lands_n; j++) {
            double d = sqrt(pow(soldiers[i].x - (soldiers[i].destination)->barrack_x, 2) + 
                            pow(soldiers[i].y - (soldiers[i].destination)->barrack_y, 2));
            if (d < 26 && soldiers[i].power > 0) {
                if (soldiers[i].side == (soldiers[i].destination)->side) {
                    (soldiers[i].destination)->soldiers += soldiers[i].power;
                    soldiers[i].power = 0;
                }
                else {
                    if ((soldiers[i].destination)->soldiers - soldiers[i].power >= 0) {
                        (soldiers[i].destination)->soldiers -= soldiers[i].power;
                        soldiers[i].power = 0;
                    } 
                    else { // change the side
                        (soldiers[i].destination)->soldiers = soldiers[i].power - (soldiers[i].destination)->soldiers;
                        (soldiers[i].destination)->side = soldiers[i].side;
                        // set new values if was impartial
                        (soldiers[i].destination)->rebirth_rate = 60;
                        (soldiers[i].destination)->rebirth_timer = 60;
                        (soldiers[i].destination)->max_soldiers = 50;
                        soldiers[i].power = 0;
                    }
                }
            }
        }
        // for () potions
    }

    // remove zero power soldiers
    int soldiers_i = 0;
    for (int i = 0; i < *soldiers_n; i++) {
        if (soldiers[i].power > 0) {
            soldiers[soldiers_i] = soldiers[i];
            soldiers_i++;
        }
    }
    *soldiers_n = soldiers_i;
}
