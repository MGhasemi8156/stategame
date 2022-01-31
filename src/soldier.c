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
        *soldiers_ptr = realloc(*soldiers_ptr, sizeof(Soldier) * (*max_soldiers + 250));
        *max_soldiers += 250;
    }
    
    // add soldiers
    for (int i = 0; i < source->soldiers; i++) {
        Soldier temp;
        
        temp.till_birth = (i/3) * 7; // spawn three at a time
        temp.born = 0;
        
        temp.x = (double)source->barrack_x;
        temp.y = (double)source->barrack_y;        

        temp.r = 5; // TODO change
        
        double l = sqrt(pow((destination->barrack_x - temp.x), 2) +
                        pow((destination->barrack_y - temp.y), 2));
        
        temp.vx = ((double)destination->barrack_x - (double)temp.x)/l * 3;
        temp.vy = ((double)destination->barrack_y - (double)temp.y)/l * 3;
        
        // move to the boundry of barrack
        temp.x += temp.vx * 10;
        temp.y += temp.vy * 10;

        // make three parallel queue
        switch (i%3) {
            case 1:
                temp.x += temp.vy * 5;
                temp.y -= temp.vx * 5;
                break;
            case 2:
                temp.x -= temp.vy * 5;
                temp.y += temp.vx * 5;
                break;
        }
        
                
        temp.power = 1;
    
        temp.side = source->side;
        temp.source = source;
        temp.destination = destination;
        
        (*soldiers_ptr)[*soldiers_n + i] = temp;
    }
    
    // attack queue
    source->attack_queue = source->soldiers;
    
    *soldiers_n += source->soldiers;
}

void apply_soldiers(SDL_Renderer* Renderer, int soldiers_n, Soldier *soldiers) {
    for (int i = 0; i < soldiers_n; i++) {
        if (!soldiers[i].born) { // birth check
            soldiers[i].till_birth -= 1;
            if (soldiers[i].till_birth <= 0) {
                soldiers[i].born = 1;
                (soldiers[i].source)->soldiers -= 1;
                (soldiers[i].source)->attack_queue -= 1;
            }
            else {
                (soldiers[i].source)->is_attaking = 1;
            }
        }
        else {
            // move
            soldiers[i].x += soldiers[i].vx;
            soldiers[i].y += soldiers[i].vy;
            // draw
            filledCircleColor(Renderer, (Sint16)soldiers[i].x, (Sint16)soldiers[i].y,
                              soldiers[i].r, get_side_normal_color(soldiers[i].side)); // TODO color on side
            
        }
    }
}


void collision_detection(int soldiers_n, Soldier *soldiers, int lands_n, Land lands[]) {
    for (int i = 0; i < soldiers_n; i++) {
        // soldiers collisions
        for (int j = i + 1; j < soldiers_n; j++) { // TODO not tested
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
        if (soldiers[i].power > 0) {
            double d = sqrt(pow(soldiers[i].x - (soldiers[i].destination)->barrack_x, 2) + 
                            pow(soldiers[i].y - (soldiers[i].destination)->barrack_y, 2));
            if (d < 26) {
                if (soldiers[i].side == (soldiers[i].destination)->side) {
                    (soldiers[i].destination)->soldiers += soldiers[i].power;
                    soldiers[i].power = 0;
                }
                else {
                    // side does not change
                    if ((soldiers[i].destination)->soldiers - soldiers[i].power >= 0) {
                        (soldiers[i].destination)->soldiers -= soldiers[i].power;
                        soldiers[i].power = 0;
                    } 
                    // side changes
                    else {
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
        // TODO potions collisions
    }
}

void remove_zero_power_soldiers(int* soldiers_n, Soldier *soldiers) {
    int soldiers_i = 0;
    for (int i = 0; i < *soldiers_n; i++) {
        if (soldiers[i].power > 0) {
            soldiers[soldiers_i] = soldiers[i];
            soldiers_i++;
        }
    }
    
    *soldiers_n = soldiers_i;
}

