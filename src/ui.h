#ifndef __HEADER_UI_H__
#define __DEADER_UI_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

SDL_Texture* create_background_texture(char image_path[50], SDL_Renderer* Renderer);

void draw_start_menu(SDL_Renderer* Renderer, char username[], char alert[]);

#endif
