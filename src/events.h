#ifndef __HEADER_EVENTS_H__
#define __HEADER_EVENTS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "map.h"

void event_listener(SDL_bool* shallExit_ptr, int land_n, Land lands[20], Land* selected_land_ptr);


#endif
