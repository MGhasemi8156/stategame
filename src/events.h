#ifndef __HEADER_EVENTS_H__
#define __HEADER_EVENTS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "map.h"
#include "soldier.h"
void event_listener(SDL_bool* shallExit_ptr, int lands_n, Land lands[20],
                    Land** selected_land_ptr, int *soldiers_n, int* max_soldiers,
                    Soldier **soldiers);

void start_menu_event_listener(SDL_bool* shall_exit_ptr, int* window_number_ptr, char username[], char alert[]);

void select_map_menu_event_listener(SDL_bool* shall_exit_ptr, int* window_number_ptr, char alert[], int maps_n, int* current_map_number_ptr, int* game_mode_ptr);

#endif
