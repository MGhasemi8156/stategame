#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "map.h"


Uint32 get_side_normal_color(int side);
Uint32 get_side_selcted_color(int side);

// 0 -> successed, 1 -> get error
int load_map(char file_path[100], int *lands_n, Land lands[20]) {
    FILE *file_ptr = fopen(file_path, "r");
    if (file_ptr == NULL) { // check for error
        fprintf(stderr, "%s does not exists.\n", file_path);
        return 1;
    }
    fscanf(file_ptr, "%d", lands_n); // read number 
    
    for (int i = 0; i < *lands_n; i++) { // read lands data
        fscanf(file_ptr, "%s %d %d %d %d %lf %d %d %hd %d", lands[i].path, &lands[i].x, &lands[i].y,
                                                    &lands[i].width, &lands[i].height, &lands[i].angle,
                                                    &lands[i].has_barrack, &lands[i].side,
                                                    &lands[i].barrack_r, &lands[i].soldiers);
        // set barrack cordinates
        lands[i].barrack_x = lands[i].x + lands[i].width/2;
        lands[i].barrack_y = lands[i].y + lands[i].height/2;
        
        lands[i].selected = 0;
    }
    fclose(file_ptr);
    return 0;
}

void apply_map(SDL_Renderer* Renderer, int lands_n, Land lands[20]) {
    for (int i = 0; i < lands_n; i++) {
        // make surface and check
        SDL_Surface* image = SDL_LoadBMP(lands[i].path);
        if (image == NULL) fprintf(stderr, "%s\n", SDL_GetError());
        // make texture and check
        SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer, image);
        if (texture == NULL) fprintf(stderr, "%s\n", SDL_GetError());
        
        SDL_Rect source = {.x = lands[i].x, .y = lands[i].y, .w = lands[i].width, .h = lands[i].height};
        // render
        SDL_RenderCopyEx(Renderer, texture, NULL, &source, lands[i].angle, NULL, SDL_FLIP_NONE);
        
        if (lands[i].has_barrack) {
            Uint32 color = lands[i].selected && lands[i].side == 1 ? 0xffede2b9: get_side_normal_color(lands[i].side);
            filledCircleColor(Renderer, lands[i].barrack_x, lands[i].barrack_y, lands[i].barrack_r, color);
            char number[5];
            sprintf(number, "%d", lands[i].soldiers);
            stringRGBA(Renderer, lands[i].barrack_x - 8, lands[i].barrack_y - 5, number, 0, 0, 0, 255);
        }
        
        // free allocated memory
        SDL_FreeSurface(image);
        SDL_DestroyTexture(texture);
        
    }
}

Uint32 get_side_normal_color(int side) {
    switch (side) {
        case 0: return 0xffcfcdcc;
        case 1: return 0xfffa863e;
        case 2: return 0xff494beb;
        case 3: return 0xff79ed8a;
        case 4: return 0xff66e3e3;
        case 5: return 0xffe366db;
        case 6: return 0xff9866e3;
        case 7: return 0xff739feb;
        default: return 0xffcfcdcc;
    }
}

