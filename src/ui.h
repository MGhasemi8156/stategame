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

void draw_start_menu(SDL_Renderer* Renderer, TTF_Font* font, char username[], char alert[]);

void draw_select_map_menu(SDL_Renderer* Renderer, TTF_Font* font, int maps_n, int current_map_number, char alert[], int global_players_n, int global_lands_n);

void draw_scoreboard(SDL_Renderer* Renderer, TTF_Font* font, int users_n, char usernames[50][20], int scores[50], char alert[]);

#endif
