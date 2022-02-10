#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <dirent.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

#include "map.h"
#include "soldier.h"
#include "potion.h"
#include "ai.h"
#include "events.h"
#include "ui.h"

#define MAX_USERNAME_LENGTH 20

#define FPS 60

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

int get_maps_n();
int get_users_number(char usernames[50][20], int scores[50]);
void add_new_user(char username[], int* users_n, char usernames[50][20], int scores[50]);
void save_users(int users_n, char usernames[50][20], int scores[50]);
void check_win(int lands_n, Land lands[], int soldiers_n, Soldier *soldiers, char username[], int users_n, char usernames[50][20], int scores[50], int* window_number, char alert[]);
void check_lose(int lands_n, Land lands[], int soldiers_n, Soldier *soldiers, char username[], int users_n, char usernames[50][20], int scores[50], int* window_number, char alert[]);
void sort_users(int users_n, char usernames[50][20], int scores[50]);

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
        
    // set rand seed
    srand(time(0));

    // users stuff
    char usernames[50][20];
    int scores[50];
    int users_n = get_users_number(usernames, scores);
    
    // set maps_n
    maps_n = get_maps_n();
    
    SDL_Window *game_window = SDL_CreateWindow("state.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                                      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);   
    SDL_Renderer *Renderer = SDL_CreateRenderer(game_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    
    TTF_Font* font = TTF_OpenFont("./assets/fonts/arial.ttf", 24);
    if (font == NULL) printf("%s\n", TTF_GetError());
    
    while (shall_exit == SDL_FALSE) {
        if (window_number == 0 || window_number == 1 || window_number == 2) {
            

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
                    draw_start_menu(Renderer, font, username, alert);
                    
                    start_menu_event_listener(&shall_exit, &window_number, username, alert);
                }    
                if (window_number == 1) {
                    add_new_user(username, &users_n, usernames, scores);                

                    draw_select_map_menu(Renderer, maps_n, current_map_number, alert, global_players_n, global_lands_n);
                    
                    select_map_menu_event_listener(&shall_exit, &window_number, alert, maps_n, &current_map_number, &game_mode, &global_lands_n, &global_players_n);
                    
                    save_users(users_n, usernames, scores);
                }
                if (window_number == 2) {
                    SDL_SetWindowFullscreen(game_window, 0);

                    add_new_user(username, &users_n, usernames, scores); 
                    
                    sort_users(users_n, usernames, scores);
                    
                    draw_scoreboard(Renderer, users_n, usernames, scores, alert);
                
                    scoreboard_event_listener(&shall_exit, &window_number, alert);

                    save_users(users_n, usernames, scores);
                }
                
                // update window
                SDL_RenderPresent(Renderer);
                // set delay as FPS
                SDL_Delay(1000 / FPS);
            }

            // free background texture
            SDL_DestroyTexture(background_texture);
        }
        if (window_number == 3) {
            SDL_SetWindowFullscreen(game_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            
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
    
            add_new_user(username, &users_n, usernames, scores); 
    
            while (shall_exit == SDL_FALSE && window_number == 3) {
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
                
                check_win(lands_n, lands, soldiers_n, soldiers, username, users_n, usernames, scores, &window_number, alert);
                check_lose(lands_n, lands, soldiers_n, soldiers, username, users_n, usernames, scores, &window_number, alert);

                // update window
                SDL_RenderPresent(Renderer);
                // set delay as FPS
                SDL_Delay(1000 / FPS);
            }
            // free allocated memory
            free(soldiers);
        }

        save_users(users_n, usernames, scores);
    }
    TTF_CloseFont(font);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(game_window);
    
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

int get_users_number(char usernames[50][20], int scores[50]) {
    FILE* users_file_ptr = fopen("./data/users.txt", "r");
    int i = 0;
    while(fscanf(users_file_ptr, "%s", usernames[i]) != EOF) {
        fscanf(users_file_ptr, "%d", &scores[i]);
        i++;
    }
    fclose(users_file_ptr);
    return i;
}

void save_users(int users_n, char usernames[50][20], int scores[50]) {
    FILE* users_file_ptr = fopen("./data/users.txt", "w");
    for (int i = 0; i < users_n; i++) {
        fprintf(users_file_ptr, "%s %d\n", usernames[i], scores[i]);
    }

    fclose(users_file_ptr);
}

void add_new_user(char username[], int* users_n, char usernames[50][20], int scores[50]) {
    for (int i = 0; i < *users_n; i++) {
        if (strcmp(username, usernames[i]) == 0)
            return;
    }   
    strcpy(usernames[*users_n], username);
    scores[*users_n] = 100;
    *users_n += 1;
}

void check_win(int lands_n, Land lands[], int soldiers_n, Soldier *soldiers, char username[], int users_n, char usernames[50][20], int scores[50], int* window_number, char alert[]) {
    for (int i = 0; i < lands_n; i++) {
        if (lands[i].side != 1 && lands[i].side != 0) return;
    }
    for (int i = 0; i < soldiers_n; i++) {
        if (soldiers[i].side != 1) return;
    }
    
    for (int i = 0; i < users_n; i++) {
        if (strcmp(username, usernames[i]) == 0) {
            scores[i] += 30;
            strcat(alert, "You win and you gain 30 points.");
            break;
        }
    }

    *window_number = 2;

}

void check_lose(int lands_n, Land lands[], int soldiers_n, Soldier *soldiers, char username[], int users_n, char usernames[50][20], int scores[50], int* window_number, char alert[]) {
    for (int i = 0; i < lands_n; i++) {
        if (lands[i].side == 1) return;
    }
    for (int i = 0; i < soldiers_n; i++) {
        if (soldiers[i].side == 1) return;
    }
    
    for (int i = 0; i < users_n; i++) {
        if (strcmp(username, usernames[i]) == 0) {
            scores[i] -= 20;
            if (scores[i] < 0) scores[i] = 0;
            strcat(alert, "You lose and you lose 20 points.");
            break;
        }
    }

    *window_number = 2;

}

void sort_users(int users_n, char usernames[50][20], int scores[50]) {
    for (int i = 0; i < users_n; i++) {
        for (int j = i + 1; j < users_n; j++) {
            if (scores[i] < scores[j]) {
                int temp_score = scores[i];
                scores[i] = scores[j];
                scores[j] = temp_score;
                char temp_username[20];
                strcpy(temp_username, usernames[i]);
                strcpy(usernames[i], usernames[j]);
                strcpy(usernames[j], temp_username);
            }
        }
    }


}
