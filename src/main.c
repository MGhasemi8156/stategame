#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <dirent.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

#include "map.h"
#include "soldier.h"
#include "potion.h"
#include "ai.h"
#include "events.h"
#include "ui.h"

#define MAX_USERNAME_LENGTH 20

#define FPS 60

int get_maps_n();

int window_number = 0; // 0 -> start menu, 2 -> score board, 1 -> select map, 3 -> game window

char username[MAX_USERNAME_LENGTH];

char alert[150];

int maps_n = 0;
int current_map_number = 1;
int game_mode = 0; // 0 -> saved maps, 1 -> generate new map

int global_players_n = 4;
int global_lands_n = 15;


SDL_bool shall_exit = SDL_FALSE;

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return 0;
    }
    if (TTF_Init() < 0) {
        fprintf(stderr, "%s\n", TTF_GetError());
        return 0;   
    }
    TTF_Init();
        
    // set rand seed
    srand(time(0));

    // set maps_n
    maps_n = get_maps_n();

    while (shall_exit == SDL_FALSE) {
        if (window_number == 0 || window_number == 1 || window_number == 2) {
            const int SCREEN_WIDTH = 1080;
            const int SCREEN_HEIGHT = 720;

            SDL_Window *game_menu = SDL_CreateWindow("state.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                                      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);   
            SDL_Renderer *Renderer = SDL_CreateRenderer(game_menu, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
            
            // background image
            SDL_Texture* background_texture = create_background_texture("./assets/background.bmp", Renderer);
            SDL_Rect background_rect = {.x = 0, .y = 0, .w = SCREEN_WIDTH, .h = SCREEN_HEIGHT};
            
            while ((window_number == 0 || window_number == 1 || window_number == 2) && shall_exit == SDL_FALSE) {
                SDL_SetRenderDrawColor(Renderer, 0xff, 0xff, 0xff, 0xff);
                SDL_RenderClear(Renderer);
                
                // render background image
                SDL_RenderCopy(Renderer, background_texture, NULL, &background_rect);
                
                // render   

                if (window_number == 0) {
                    draw_start_menu(Renderer, username, alert);
                    
                    start_menu_event_listener(&shall_exit, &window_number, username, alert);
                }    
                if (window_number == 1) {
                    draw_select_map_menu(Renderer, maps_n, current_map_number, alert);
                    
                    select_map_menu_event_listener(&shall_exit, &window_number, alert, maps_n, &current_map_number, &game_mode);
                }
                if (window_number == 2) {
                }
                
                // update window
                SDL_RenderPresent(Renderer);
                // set delay as FPS
                SDL_Delay(1000 / FPS);
            }

            // free background texture
            SDL_DestroyTexture(background_texture);
            
            SDL_DestroyRenderer(Renderer);
            SDL_DestroyWindow(game_menu);
        }
        if (window_number == 3) {
            const int SCREEN_WIDTH = 640;
            const int SCREEN_HEIGHT = 480;
            
            SDL_Window *Game_Window = SDL_CreateWindow("state.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                             SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
            SDL_Renderer *Renderer = SDL_CreateRenderer(Game_Window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

            int lands_n;
            Land lands[MAX_LANDS];

            if (game_mode == 0) {
                char map_path[150];
                sprintf(map_path, "./data/maps/map%d.txt", current_map_number);
                load_rand_map(map_path, &lands_n, lands);
            }
            else if (game_mode == 1) {
                lands_n = global_lands_n;
                create_rand_map(lands_n, lands, global_players_n);
                current_map_number = maps_n + 1;
            }
            Land* selected_land = NULL;

            Soldier *soldiers = malloc(250 * sizeof(Soldier));
            int soldiers_n = 0;
            int max_soldiers = 250;

            Potion potions[MAX_POTIONS];
            int potions_n = 0;   
    
   
    
            while (shall_exit == SDL_FALSE) {
                // renderer color and clear
                SDL_SetRenderDrawColor(Renderer, 0xff, 0xff, 0xff, 0xff);
                SDL_RenderClear(Renderer);
        
                apply_rand_map(Renderer, lands_n, lands, selected_land);
        

                add_potion(&potions_n, potions, lands_n, lands);
                apply_potions(Renderer, potions_n, potions, lands_n, lands, soldiers_n, soldiers);
                remove_expired_potions(&potions_n, potions);
        
                apply_soldiers(Renderer, soldiers_n, soldiers);
       
                collision_detection(soldiers_n, soldiers, lands_n, lands, potions_n, potions);
        
                remove_zero_power_soldiers(&soldiers_n, soldiers);        
        
                check_bot_attack(lands_n, lands, &soldiers_n, &max_soldiers, &soldiers);
        
                // listen for key events
                event_listener(&shall_exit, lands_n, lands, &selected_land, &soldiers_n,
                               &max_soldiers, &soldiers);

                // update window
                SDL_RenderPresent(Renderer);
                // set delay as FPS
                SDL_Delay(1000 / FPS);
            }
            // free allocated memory
            free(soldiers);
            SDL_DestroyRenderer(Renderer);
            SDL_DestroyWindow(Game_Window);
        }
    }

    return 0;
}

int get_maps_n() {
    DIR *maps_dir;
    struct dirent *dir;
    maps_dir = opendir("./data/maps");
    if (maps_dir == NULL) fprintf(stderr, "Can't open dir ./data/maps\n");
    
    int maps_n = 0;
    if (maps_dir != NULL) {
        while ((dir = readdir(maps_dir)) != NULL) {
            maps_n += 1;
        }
        maps_n -= 2; // for . and ..
    }
    
    free(maps_dir);
    return maps_n;
}

