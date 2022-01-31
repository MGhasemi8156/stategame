#ifndef __HEADER_AI_H__
#define __HEADER_AI_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "map.h"
#include "soldier.h"

void check_bot_attack(int lands_n, Land lands[], int* soldiers_n,
                      int* max_soldiers, Soldier **soldiers_ptr);


#endif
