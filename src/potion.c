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

void increase_velocity(int target_side, int soldiers_n, Soldier *soldiers);
void end_increase_velocity(int target_side, int soldiers_n, Soldier *soldiers);

void unlimited_rebirth(int target_side, int lands_n, Land lands[]);
void end_unlimited_rebirth(int target_side, int lands_n, Land lands[]);

void add_potion(int* potions_n, Potion potions[], int lands_n, Land lands[]) {
    if (*potions_n < MAX_POTIONS) {
        for (int i = 0; i < lands_n; i++) {
            for (int j = i + 1; j < lands_n; j++) {
                if (rand()%RAND_CREATION_RATE == 0) {
                    double rx = lands[j].barrack_x - lands[i].barrack_x;
                    double ry = lands[j].barrack_y - lands[i].barrack_y;
                    
                    // refer to place of potion between tow lands
                    double rand_factor = ((double)(rand()%5 + 1)) / 6;
                    
                    Potion temp;
                    temp.x = lands[i].barrack_x + rx * rand_factor;
                    temp.y = lands[i].barrack_y + ry * rand_factor;
                    
                    temp.type = 3; // TODO change randonm value
                    temp.target_side = -1;
                    temp.till_end = 600; // TODO change
                    
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
        Sint16 vx[POTION_SIDES];
        Sint16 vy[POTION_SIDES];
        for (int k = 0; k < POTION_SIDES; k++) {
            vx[k] = potions[i].x + cos(PI/(POTION_SIDES / 2) * k) * POTION_R;
            vy[k] = potions[i].y + sin(PI/(POTION_SIDES / 2) * k) * POTION_R;
        }
        if (potions[i].target_side == -1) {
            filledPolygonColor(Renderer, vx, vy, POTION_SIDES, 0xaaff0000);
        }
        else {
            filledPolygonColor(Renderer, vx, vy, POTION_SIDES, 0x55ff0000);
            potions[i].till_end -= 1;
            if (potions[i].till_end > 0) {
                switch (potions[i].type) {
                    case 1:
                        stop_enemies(potions[i].target_side, soldiers_n, soldiers);
                        break;
                    case 2:
                        increase_velocity(potions[i].target_side, soldiers_n, soldiers);
                        break;
                    case 3:
                        unlimited_rebirth(potions[i].target_side, lands_n, lands);
                        break;
                }
            }
            else {
                switch (potions[i].type) {
                    case 1:
                        end_stop_enemies(potions[i].target_side, soldiers_n, soldiers);
                        break;
                    case 2:
                        end_increase_velocity(potions[i].target_side, soldiers_n, soldiers);
                        break;
                    case 3:
                        end_unlimited_rebirth(potions[i].target_side, lands_n, lands);
                        break;
                }
            }
        }
    }
}

void remove_expired_potions(int* potions_n, Potion potions[]) {
    int potions_i = 0;  
    for (int i = 0; i < *potions_n; i++) {
        if (potions[i].till_end > 0) {
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

void increase_velocity(int target_side, int soldiers_n, Soldier *soldiers) {
    for (int i = 0; i < soldiers_n; i++) {
        if (soldiers[i].side == target_side) soldiers[i].velocity_factor = INCREASE_VELOCITY_FACTOR;
    }
}

void end_increase_velocity(int target_side, int soldiers_n, Soldier *soldiers) {
    for (int i = 0; i < soldiers_n; i++) {
        if (soldiers[i].side == target_side) soldiers[i].velocity_factor = 1;
    }

}

void unlimited_rebirth(int target_side, int lands_n, Land lands[]) {
    for (int i = 0; i < lands_n; i++) {
        if (lands[i].side == target_side) lands[i].unlimited_rebirth = 1;
    }
}

void end_unlimited_rebirth(int target_side, int lands_n, Land lands[]) {
    for (int i = 0; i < lands_n; i++) {
        if (lands[i].side == target_side) lands[i].unlimited_rebirth = 0;
    }
}
