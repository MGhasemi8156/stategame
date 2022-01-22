#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "map.h"
#include "events.h"

void event_listener(SDL_bool* shallExit_ptr, int land_n, Land lands[20], Land* selected_land_ptr) {
    SDL_Event Event;
    while (SDL_PollEvent(&Event)) {
        switch (Event.type) {
            case SDL_QUIT:
                *shallExit_ptr = SDL_TRUE;
                break;
        }
    }
    return;
}
