#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "map.h"
#include "events.h"

void event_listener(SDL_bool* shallExit_ptr, int lands_n, Land lands[20], Land** selected_land_ptr) {
    SDL_Event Event;
    while (SDL_PollEvent(&Event)) {
        switch (Event.type) {
            case SDL_QUIT:
                *shallExit_ptr = SDL_TRUE;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (Event.button.button == SDL_BUTTON_LEFT) {
                    for (int i = 0; i < lands_n; i++) {
                        double d = sqrt(pow(lands[i].barrack_x - Event.button.x, 2) + 
                                        pow(lands[i].barrack_y - Event.button.y, 2));
                        if (d <= lands[i].barrack_r + 5) {
                            if (*selected_land_ptr == NULL) {
                                if (lands[i].side == 1) { // only player could be the selected
                                    *selected_land_ptr = &lands[i];
                                    (*selected_land_ptr)->selected = 1;
                                }
                            }
                            break;
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (Event.button.button == SDL_BUTTON_LEFT) {
                    if (*selected_land_ptr != NULL) {
                        for (int i = 0; i < lands_n; i++) {
                            double d = sqrt(pow(lands[i].barrack_x - Event.button.x, 2) + 
                                            pow(lands[i].barrack_y - Event.button.y, 2));
                            if (d <= lands[i].barrack_r + 5) {
                                if (*selected_land_ptr != &lands[i]) {
                                    printf("move from %d-%d to %d-%d\n", (*selected_land_ptr)->x, (*selected_land_ptr)->y, lands[i].x, lands[i].y);
                                }   
                                break;
                            }
                        }
                        (*selected_land_ptr)->selected = 0;
                        *selected_land_ptr = NULL;
                    }
                }
                break;
        }
    }
    return;
}
