#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "map.h"
#include "soldier.h"
#include "events.h"

int check_username(char username[]);

void event_listener(SDL_bool* shallExit_ptr, int lands_n, Land lands[20],
                    Land** selected_land_ptr, int* soldiers_n, int* max_soldiers,
                    Soldier **soldiers) {
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
                                if (lands[i].side == 1 && !lands[i].is_attaking) { // only player could be the selected
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
                                    add_soldiers(soldiers_n, max_soldiers, soldiers,
                                                 *selected_land_ptr, &lands[i]);
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

void start_menu_event_listener(SDL_bool* shall_exit_ptr, int* window_number_ptr, char username[], char alert[]) {
    SDL_Event Event;
    while(SDL_PollEvent(&Event)) {
        switch (Event.type) {
            case SDL_QUIT:
                *shall_exit_ptr = SDL_TRUE;
                break;
            case SDL_TEXTINPUT:
                if (strlen(username) + 1 < 20) // change to const
                    strcat(username, Event.text.text);
                break;
            case SDL_KEYDOWN:
                if (Event.key.keysym.sym == SDLK_BACKSPACE)
                if (strlen(username) > 0) username[strlen(username) - 1] = '\0';
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (Event.button.x >= 100 && Event.button.x <= 370 && Event.button.y >= 400 && Event.button.y <= 480) {
                    if (check_username(username) == -1)
                        sprintf(alert, "Username can not include spaces");
                    else if (check_username(username) == -2)
                        sprintf(alert, "You must enter username");
                    else {
                        *window_number_ptr = 1; // go to select map
                        sprintf(alert, "");
                    }
                }
                if (Event.button.x >= 135 && Event.button.x <= 405 && Event.button.y >= 495 && Event.button.y <= 575) {
                    if (check_username(username) == -1)
                        sprintf(alert, "Username can not include spaces");
                    else if (check_username(username) == -2)
                        sprintf(alert, "You must enter username");
                    else {
                        *window_number_ptr = 0; // TODO continue save game
                        sprintf(alert, "");
                    }
                }
                if (Event.button.x >= 175 && Event.button.x <= 440 && Event.button.y >= 590 && Event.button.y <= 670) {
                    if (check_username(username) == -1)
                        sprintf(alert, "Username can not include spaces");
                    else if (check_username(username) == -2)
                        sprintf(alert, "You must enter username");
                    else {
                        *window_number_ptr = 2; // go to score board
                        sprintf(alert, "");
                    }
                }
        }
    }
}

void select_map_menu_event_listener(SDL_bool* shall_exit_ptr, int* window_number_ptr, char alert[], int maps_n, int* current_map_number_ptr, int* game_mode_ptr) {
    SDL_Event Event;
    while(SDL_PollEvent(&Event)) {
        switch (Event.type) {
            case SDL_QUIT:
                *shall_exit_ptr = SDL_TRUE;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (Event.button.x >= 150 && Event.button.x <= 200 && Event.button.y >= 160 &&
                    Event.button.y <= 240 && *current_map_number_ptr > 1) 
                    *current_map_number_ptr -=1;
                if (Event.button.x >= 880 && Event.button.x <= 930 && Event.button.y >= 160 &&
                    Event.button.y <= 240 && *current_map_number_ptr < maps_n) 
                    *current_map_number_ptr +=1;
                if (Event.button.x >= 390 && Event.button.x <= 690 && Event.button.y >= 400 &&
                    Event.button.y <= 500 && maps_n > 0) {
                    *game_mode_ptr = 0;
                    *window_number_ptr = 3;
                }
                if (Event.button.x >= 390 && Event.button.x <= 690 && Event.button.y >= 500 &&
                    Event.button.y <= 580) {
                    *game_mode_ptr = 1;
                    *window_number_ptr = 3;
                }
                break;
        }
    }

}

// 0 -> valid -1 -> invalid
int check_username(char username[]) {
    for (int i = 0; i < strlen(username); i++)
        if (username[i] == ' ') return -1;
    if (strlen(username) == 0) return -2;
    return 0;
}

