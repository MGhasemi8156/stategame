#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "map.h"
#include "soldier.h"
#include "potion.h"

#define PI 3.14159265

void stop_enemies(int target_side, int soldiers_n, Soldier *soldiers);

void end_stop_enemies(int target_side, int soldiers_n, Soldier *soldiers);

void add_potion(int* potions_n, Potion potions[], int lands_n, Land lands[]) {
    if (*potions_n < MAX_POTIONS) {
        for (int i = 0; i < lands_n; i++) {
            for (int j = i + 1; j < lands_n; j++) {
                if (rand()%60000 == 0) {
                    double rx = lands[j].barrack_x - lands[i].barrack_x;
                    double ry = lands[j].barrack_y - lands[i].barrack_y;
                    
                    double rand_factor = ((double)(rand()%5 + 1)) / 6;
                    
                    Potion temp;
                    temp.x = lands[i].barrack_x + rx * rand_factor;
                    temp.y = lands[i].barrack_y + ry * rand_factor;
                    temp.n = 10;
                    
                    temp.type = 1; // TODO change randonm value
                    temp.target_side = -1;
                    temp.life_time = 600; // TODO change
                    
                    potions[*potions_n] = temp;
                    *potions_n += 1;
                    return;
                }
            }
        }
    }
}

void apply_potions(SDL_Renderer* Renderer, int postions_n, Potion potions[],
                   int lands_n, Land lands[], int soldiers_n, Soldier *soldiers) {
    for (int i = 0; i < postions_n; i++) {
        Sint16 vx[6];
        Sint16 vy[6];
        for (int k = 0; k < 6; k++) {
            vx[k] = potions[i].x + cos(PI/3 * k) * 20;
            vy[k] = potions[i].y + sin(PI/3 * k) * 20;
        }
        if (potions[i].target_side == -1) {
            filledPolygonColor(Renderer, vx, vy, 6, 0xaaff0000);
        }
        else {
            filledPolygonColor(Renderer, vx, vy, 6, 0x55ff0000);
            potions[i].life_time -= 1;
            if (potions[i].life_time > 0) {
                switch (potions[i].type) {
                    case 1:
                        stop_enemies(potions[i].target_side, soldiers_n, soldiers);
                        break;
                }
            }
            else {
                switch (potions[i].type) {
                    case 1:
                        end_stop_enemies(potions[i].target_side, soldiers_n, soldiers);
                        break;
                }
            }
        }
    }
}

void remove_expired_potions(int* potions_n, Potion potions[]) {
    int potions_i = 0;  
    for (int i = 0; i < *potions_n; i++) {
        if (potions[i].life_time > 0) {
            potions[potions_i] = potions[i];
            potions_i++;
        }
    }
    
    *potions_n = potions_i;
}


void stop_enemies(int target_side, int soldiers_n, Soldier *soldiers) {
    for (int i = 0; i < soldiers_n; i++) {
        if (soldiers[i].side != target_side) soldiers[i].can_move = 0;
    }
}

void end_stop_enemies(int target_side, int soldiers_n, Soldier *soldiers) {
    for (int i = 0; i < soldiers_n; i++) {
        if (soldiers[i].side != target_side) soldiers[i].can_move = 1;
    }
}
