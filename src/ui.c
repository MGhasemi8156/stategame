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
    return background_texture;
}

void draw_start_menu(SDL_Renderer* Renderer, char username[], char alert[]) {
    // add fonts
    TTF_Font* font = TTF_OpenFont("/home/mohsen/Projects/state.io/assets/fonts/arial.ttf", 24);
    
    if (font == NULL) {
       // fprintf(stderr, "Can't open afds\n");
        //return;
    }    

    SDL_Color text_color = {0, 0, 0};
    
    // input form
    boxColor(Renderer, 40, 275, 340, 355, 0xcceb6e34);
    boxColor(Renderer, 43, 278, 337, 352, 0xffffffff);

    stringColor(Renderer, 45, 300, username, 0xff000000);

    stringColor(Renderer, 45, 370, alert, 0xff000000);
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
    }
    else filledPolygonColor(Renderer, button1_vx, button1_vy, 4, 0xffffad42);
    
    if (mouse_x <= button2_vx[3] && mouse_x >= button2_vx[1] && mouse_y >= button2_vy[0] && mouse_y <= button2_vy[2]) {
        for (int i = 0; i < 4; i++) button2_vx[i] += 30;
        filledPolygonColor(Renderer, button2_vx, button2_vy, 4, 0xffff9100);
    }
    else filledPolygonColor(Renderer, button2_vx, button2_vy, 4, 0xffffad42);

    if (mouse_x <= button3_vx[3] && mouse_x >= button3_vx[1] && mouse_y >= button3_vy[0] && mouse_y <= button3_vy[2]) {
        for (int i = 0; i < 4; i++) button3_vx[i] += 30;
        filledPolygonColor(Renderer, button3_vx, button3_vy, 4, 0xffff9100);
    }
    else filledPolygonColor(Renderer, button3_vx, button3_vy, 4, 0xffffad42);

    SDL_Rect input_form_rect = {.x = 35, .y = 310, .w = 400, .h = 100};

    // input form text
    //SDL_Surface* input_form_text_surface = TTF_RenderText_Solid(font, username, text_color);
    //SDL_Texture* input_form_text_texture = SDL_CreateTextureFromSurface(Renderer, input_form_text_surface);
    //SDL_Rect input_form_text_rect = {.x = 35, .y = 310};
    //SDL_QueryTexture(input_form_text_texture, NULL, NULL, &input_form_text_rect.w, &input_form_text_rect.h);
    
    // render stuff
    //SDL_RenderCopy(Renderer, input_form_text_texture, NULL, &input_form_text_rect);
    
    // free stuff
    //SDL_FreeSurface(input_form_text_surface);
    //SDL_DestroyTexture(input_form_text_texture);
    
}


void draw_select_map_menu(SDL_Renderer* Renderer, int maps_n, int current_map_number, char alert[]) {
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
   
        boxColor(Renderer, 390, 400, 690, 500, 0xffffffff);
        stringColor(Renderer, 540, 450, " start game", 0xff000000);

        
        
        
        SDL_RenderCopy(Renderer, map_texture, NULL, &map_rect);
        


        // free map screenshot stuff
        SDL_FreeSurface(map_surface);
        SDL_DestroyTexture(map_texture);
    }

    boxColor(Renderer, 390, 500, 690, 580, 0xff00ffff);
    stringColor(Renderer, 540, 540, "start a rand map", 0xff000000);
}

