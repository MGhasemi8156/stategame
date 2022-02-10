#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

#include "ui.h"

void draw_text_cursor(SDL_Renderer* Renderer, int w);

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
    return background_texture;
}

void draw_start_menu(SDL_Renderer* Renderer, TTF_Font* font, char username[], char alert[]) {
    // texts
    SDL_Color text_color = {0, 0, 0};
        
    SDL_Surface* username_surface = TTF_RenderText_Solid(font, "Username:", text_color);
    SDL_Texture* username_texture = SDL_CreateTextureFromSurface(Renderer, username_surface);
    SDL_Rect username_rect = {.x = 45, .y = 230};
    TTF_SizeText(font, "Username:", &username_rect.w, &username_rect.h);

    SDL_Surface* input_form_surface = TTF_RenderText_Solid(font, username, text_color);
    SDL_Texture* input_form_texture = SDL_CreateTextureFromSurface(Renderer, input_form_surface);
    SDL_Rect input_form_rect = {.x = 75, .y = 300};
    TTF_SizeText(font, username, &input_form_rect.w, &input_form_rect.h);
    
    SDL_Surface* select_map_surface = TTF_RenderText_Solid(font, "New game", text_color);
    SDL_Texture* select_map_texture = SDL_CreateTextureFromSurface(Renderer, select_map_surface);
    SDL_Rect select_map_rect = {.x = 150, .y = 425};
    TTF_SizeText(font, "New game", &select_map_rect.w, &select_map_rect.h);
    
    SDL_Surface* continue_surface = TTF_RenderText_Solid(font, "Continue(Unavailable)", text_color);
    SDL_Texture* continue_texture = SDL_CreateTextureFromSurface(Renderer, continue_surface);
    SDL_Rect continue_rect = {.x = 135, .y = 520};
    TTF_SizeText(font, "Continue(Unavailable)", &continue_rect.w, &continue_rect.h);
    
    SDL_Surface* scoreboard_surface = TTF_RenderText_Solid(font, "Scoreboard", text_color);
    SDL_Texture* scoreboard_texture = SDL_CreateTextureFromSurface(Renderer, scoreboard_surface);
    SDL_Rect scoreboard_rect = {.x = 210, .y = 615};
    TTF_SizeText(font, "Scoreboard", &scoreboard_rect.w, &scoreboard_rect.h);
    
    SDL_Color alert_color = {255, 0, 0};
    SDL_Surface* alert_surface = TTF_RenderText_Solid(font, alert, alert_color);
    SDL_Texture* alert_texture = SDL_CreateTextureFromSurface(Renderer, alert_surface);
    SDL_Rect alert_rect = {.x = 45, .y = 365};
    TTF_SizeText(font, alert, &alert_rect.w, &alert_rect.h);
    
    SDL_RenderCopy(Renderer, username_texture, NULL, &username_rect);

    // input form
    boxColor(Renderer, 40, 275, 340, 355, 0xcceb6e34);
    boxColor(Renderer, 43, 278, 337, 352, 0xffffffff);

    SDL_RenderCopy(Renderer, input_form_texture, NULL, &input_form_rect);
    
    draw_text_cursor(Renderer, input_form_rect.w);
    
    SDL_RenderCopy(Renderer, alert_texture, NULL, &alert_rect);
    // buttons
    Sint16 button1_vx[4] = {340, 40, 70, 370};
    Sint16 button1_vy[4] = {400, 400, 480, 480};
    Sint16 button2_vx[4] = {375, 75, 105, 405};
    Sint16 button2_vy[4] = {495, 495, 575, 575};
    Sint16 button3_vx[4] = {410, 110, 140, 440};
    Sint16 button3_vy[4] = {590, 590, 670, 670};
    
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    if (mouse_x <= button1_vx[3] && mouse_x >= button1_vx[1] && mouse_y >= button1_vy[0] && mouse_y <= button1_vy[2]) {
        for (int i = 0; i < 4; i++) button1_vx[i] += 30;
        filledPolygonColor(Renderer, button1_vx, button1_vy, 4, 0xffff9100);
        select_map_rect.x += 30;
    }
    else filledPolygonColor(Renderer, button1_vx, button1_vy, 4, 0xffffad42);
    SDL_RenderCopy(Renderer, select_map_texture, NULL, &select_map_rect);        

    if (mouse_x <= button2_vx[3] && mouse_x >= button2_vx[1] && mouse_y >= button2_vy[0] && mouse_y <= button2_vy[2]) {
        for (int i = 0; i < 4; i++) button2_vx[i] += 30;
        continue_rect.x += 30;
        filledPolygonColor(Renderer, button2_vx, button2_vy, 4, 0xffff9100);
    }
    else filledPolygonColor(Renderer, button2_vx, button2_vy, 4, 0xffffad42);
    SDL_RenderCopy(Renderer, continue_texture, NULL, &continue_rect);

    if (mouse_x <= button3_vx[3] && mouse_x >= button3_vx[1] && mouse_y >= button3_vy[0] && mouse_y <= button3_vy[2]) {
        for (int i = 0; i < 4; i++) button3_vx[i] += 30;
        scoreboard_rect.x += 30;
        filledPolygonColor(Renderer, button3_vx, button3_vy, 4, 0xffff9100);
    }
    else filledPolygonColor(Renderer, button3_vx, button3_vy, 4, 0xffffad42);
    SDL_RenderCopy(Renderer, scoreboard_texture, NULL, &scoreboard_rect);
    // free stuff
    SDL_DestroyTexture(input_form_texture);
    SDL_FreeSurface(input_form_surface);
    
    SDL_DestroyTexture(select_map_texture);
    SDL_FreeSurface(select_map_surface);
    
    SDL_DestroyTexture(continue_texture);
    SDL_FreeSurface(continue_surface);
    
    SDL_DestroyTexture(scoreboard_texture);
    SDL_FreeSurface(scoreboard_surface);

    SDL_DestroyTexture(alert_texture);
    SDL_FreeSurface(alert_surface);

    SDL_DestroyTexture(username_texture);
    SDL_FreeSurface(username_surface);
}

void draw_text_cursor(SDL_Renderer* Renderer, int w) {
    static int blinker = 0;
    if (blinker%60 > 30) boxColor(Renderer, 75 + w, 305, 80 + w, 325, 0xff000000);
    blinker++;
}


void draw_select_map_menu(SDL_Renderer* Renderer, TTF_Font* font, int maps_n, int current_map_number, char alert[], int global_players_n, int global_lands_n) {
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    if (maps_n > 0) {
        char image_path[100];
        sprintf(image_path, "./data/screenshots/map%d.bmp", current_map_number);
        
        SDL_Surface* map_surface = SDL_LoadBMP(image_path);
        if (map_surface == NULL) {
            fprintf(stderr, "%s\n", SDL_GetError());
            return;
        }
        SDL_Texture* map_texture = SDL_CreateTextureFromSurface(Renderer, map_surface);
        
        SDL_Rect map_rect = {.x = 220, .y = 50, .w = 640, .h = 300};
        
        if (current_map_number > 1)
            filledTrigonColor(Renderer, 200, 160, 200, 240, 150, 200, 0xff000000);
        if (current_map_number < maps_n)
            filledTrigonColor(Renderer, 880, 160, 880, 240, 930, 200, 0xff000000);
        
        if (mouse_x >= 220 && mouse_x <= 860 && mouse_y >= 50 && mouse_y <= 350)
            boxColor(Renderer, 215, 45, 865, 355, 0xffff9100);
            
        SDL_RenderCopy(Renderer, map_texture, NULL, &map_rect);
        
        // free map screenshot stuff
        SDL_FreeSurface(map_surface);
        SDL_DestroyTexture(map_texture);
    }
    
    SDL_Color text_color = {0, 0, 0};

    SDL_Surface* create_button_surface = TTF_RenderText_Solid(font, "Create a random map", text_color);
    SDL_Texture* create_button_texture = SDL_CreateTextureFromSurface(Renderer, create_button_surface);
    SDL_Rect create_button_rect = {.x = 610, .y = 483};
    TTF_SizeText(font, "Create a random map", &create_button_rect.w, &create_button_rect.h);

    SDL_Surface* return_button_surface = TTF_RenderText_Solid(font, "Return to main menu", text_color);
    SDL_Texture* return_button_texture = SDL_CreateTextureFromSurface(Renderer, return_button_surface);
    SDL_Rect return_button_rect = {.x = 425, .y = 625};
    TTF_SizeText(font, "Return to main menu", &return_button_rect.w, &return_button_rect.h);

    stringColor(Renderer, 150, 500, "Lands:", 0xff000000);
    boxColor(Renderer, 210, 470, 270, 530, 0xffffffff);
    char lands_number[5];
    sprintf(lands_number, "%d", global_lands_n);
    stringColor(Renderer, 232, 497, lands_number, 0xff000000);
    if (global_lands_n < 20)
        filledTrigonColor(Renderer, 280, 495, 300, 495, 290, 475, 0xff000000);
    if (global_lands_n > 10)
        filledTrigonColor(Renderer, 280, 505, 300, 505, 290, 525, 0xff000000);
    
    stringColor(Renderer, 340, 500, "Players:", 0xff000000);
    boxColor(Renderer, 410, 470, 470, 530, 0xffffffff);
    char players_number[5];
    sprintf(players_number, "%d", global_players_n);
    stringColor(Renderer, 432, 497, players_number, 0xff000000);
    if (global_players_n < 6)
        filledTrigonColor(Renderer, 480, 495, 500, 495, 490, 475, 0xff000000);
    if (global_players_n > 2)
        filledTrigonColor(Renderer, 480, 505, 500, 505, 490, 525, 0xff000000);
    
    if (mouse_x >= 600 && mouse_x <= 850 && mouse_y >= 470 && mouse_y <= 530)
        boxColor(Renderer, 600, 470, 850, 530, 0xffff9100);
    else boxColor(Renderer, 600, 470, 850, 530, 0xffffad42);
    SDL_RenderCopy(Renderer, create_button_texture, NULL, &create_button_rect);

    if (mouse_x >= 415 && mouse_x <= 665 && mouse_y >= 600 && mouse_y <= 680)
        boxColor(Renderer, 415, 610, 665, 670, 0xffff9100);
    else boxColor(Renderer, 415, 610, 665, 670, 0xffffad42);
    SDL_RenderCopy(Renderer, return_button_texture, NULL, &return_button_rect);

    // free
    SDL_DestroyTexture(create_button_texture);
    SDL_FreeSurface(create_button_surface);

    SDL_DestroyTexture(return_button_texture);
    SDL_FreeSurface(return_button_surface);
}


void draw_scoreboard(SDL_Renderer* Renderer, TTF_Font* font, int users_n, char usernames[50][20], int scores[50], char alert[]) {
    SDL_Color alert_color = {0, 0, 0};
    
    if (strstr(alert, "30") != NULL) alert_color.g = 255;
    if (strstr(alert, "20") != NULL) alert_color.r = 255;

    SDL_Surface* alert_surface = TTF_RenderText_Solid(font, alert, alert_color);
    SDL_Texture* alert_texture = SDL_CreateTextureFromSurface(Renderer, alert_surface);
    SDL_Rect alert_rect = {.x = 400, .y = 75};
    TTF_SizeText(font, alert, &alert_rect.w, &alert_rect.h);
    
    SDL_Color text_color = {0, 0, 0};
    SDL_Surface* return_surface = TTF_RenderText_Solid(font, "Return to main menu", text_color);
    SDL_Texture* return_texture = SDL_CreateTextureFromSurface(Renderer, return_surface);
    SDL_Rect return_rect = {.x = 430, .y = 627};
    TTF_SizeText(font, "Return to main menu", &return_rect.w, &return_rect.h);



    SDL_RenderCopy(Renderer, alert_texture, NULL, &alert_rect);
    
    boxColor(Renderer, 375, 120, 705, 600, 0xffff9100);
    boxColor(Renderer, 380, 125, 700, 595, 0xffffffff);
    
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    for (int i = 0; i < (users_n < 20 ? users_n: 20); i++) { // first 15 users
        stringColor(Renderer, 420, 150 + i * 20, usernames[i], 0xff000000);
        char score[50];
        sprintf(score, "%d", scores[i]);
        stringColor(Renderer, 630, 150 + i * 20, score, 0xff000000);
    }
    if (mouse_x >= 415&& mouse_x <= 665 && mouse_y >= 610 && mouse_y <= 670)
        boxColor(Renderer, 415, 610, 665, 670, 0xffff9100);
    else boxColor(Renderer, 415, 610, 665, 670, 0xffffad42);

    SDL_RenderCopy(Renderer, return_texture, NULL, &return_rect);
    
    // free 
    SDL_DestroyTexture(alert_texture);
    SDL_FreeSurface(alert_surface);

    SDL_DestroyTexture(return_texture);
    SDL_FreeSurface(return_surface);
}
