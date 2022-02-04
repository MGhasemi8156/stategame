#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

#include "ui.h"

SDL_Texture* create_background_texture(char image_path[50], SDL_Renderer* Renderer) {
    SDL_Surface* background_image = SDL_LoadBMP(image_path);
    if (background_image == NULL) {
        fprintf(stderr, "Can't find %s\n", image_path);
        return 0;
    }
    SDL_Texture* background_texture = SDL_CreateTextureFromSurface(Renderer, background_image);
    if (background_texture == NULL) {
        fprintf(stderr, "Can't make texture of %s\n", image_path);
        return 0;
    }
    
    SDL_FreeSurface(background_image);
    background_image = NULL;
    return background_texture;
}

