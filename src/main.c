#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "map.h"
#include "events.h"


const int FPS = 60;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



int main() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "%s", SDL_GetError());
        return 0;
    }

    SDL_Window *Game_Window = SDL_CreateWindow("state.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                             SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_Renderer *Renderer = SDL_CreateRenderer(Game_Window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // set seed
    srand(time(0));

    int lands_n = 12;
    Land lands[20];
    //create_rand_map(lands_n, lands, 3);
    load_rand_map("./data/maps/map02.txt", &lands_n, lands);

    Land* selected_land = NULL;

    SDL_bool shallExit = SDL_FALSE;

    while (shallExit == SDL_FALSE) {
        // renderer color and clear
        SDL_SetRenderDrawColor(Renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(Renderer);

        apply_rand_map(Renderer, lands_n, lands, selected_land);

        // listen for key events
        event_listener(&shallExit, lands_n, lands, &selected_land);

        // update window
        SDL_RenderPresent(Renderer);
        // set delay as FPS
        SDL_Delay(1000 / FPS);
    }

    // free allocated memory
    // TODO free window and renderer
    return 0;
}
