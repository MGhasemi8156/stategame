#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <dirent.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "map.h"


void rand_BFS(int iterator, int map[100][100], int to_check[500][2], int n);
void save_rand_land(FILE *map_file_ptr, int map[100][100], int x, int y, int has_barrack, int barrack_r,
                    int side, int soldiers, int max_soldiers, int rebirth_rate, int rebirth_timer);
void smooth_map(int map[100][100]);

char* get_new_file_name(); 
void get_land_coordinates(int* x, int* y);

Uint32 get_side_normal_color(int side);
Uint32 get_land_normal_color(int side);

void draw_attack_line(SDL_Renderer* Renderer, Land *selected_land_ptr);




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

void apply_map(SDL_Renderer* Renderer, int lands_n, Land lands[20], Land* selected_land_ptr) {
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

        // draw attack line
        draw_attack_line(Renderer, selected_land_ptr);

        // free allocated memory
        SDL_FreeSurface(image);
        SDL_DestroyTexture(texture);

    }
}

void create_rand_map(int lands_n, Land lands[], int players) {
    // open file to save rand map
    char *file_name = get_new_file_name();
    FILE* map_file_ptr = fopen(file_name, "w");
    free(file_name);
    
    // add number of lands to file
    fprintf(map_file_ptr, "%d\n", lands_n);
    
    int share = lands_n/(players + 2);
    int lands_i = 0;
    int x, y;
    
    for (int i = 1; i < players + 1; i++) { // i refers to side
        for (int j = 0; j < share; j++) {
            int map[100][100] = {0};
            int to_check[500][2] = {{50, 50}};

            rand_BFS(0, map, to_check, 1);
            smooth_map(map);        
    
            memcpy(lands[lands_i].pixels, map, 100 * 100 * sizeof(int));
            
            lands[lands_i].has_barrack = 1;
            lands[lands_i].side = i;
            
            get_land_coordinates(&x, &y);
            lands[lands_i].barrack_x = x;
            lands[lands_i].barrack_y = y;
            lands[lands_i].barrack_r = 25;
            
            lands[lands_i].soldiers = 12; // TODO change later
            lands[lands_i].max_soldiers = 50;
            lands[lands_i].rebirth_rate = 60;
            lands[lands_i].rebirth_timer = 60;
           
             lands[lands_i].selected = 0;
            
            // save land
            save_rand_land(map_file_ptr, map, lands[lands_i].barrack_x, lands[lands_i].barrack_y, lands[lands_i].has_barrack,
                           lands[lands_i].barrack_r, lands[lands_i].side, lands[lands_i].soldiers, lands[lands_i].max_soldiers,
                           lands[lands_i].rebirth_rate, lands[lands_i].rebirth_timer);
            lands_i++;
        }
    }
    
    // lands with no side
    int impartials = lands_n - lands_i;
    for (int i = 0; i < impartials; i++) {
        int map[100][100] = {0};
        int to_check[500][2] = {{50, 50}};       

        rand_BFS(0, map, to_check, 1);
        smooth_map(map);

        memcpy(lands[lands_i].pixels, map, 100 * 100 * sizeof(int));
        
        lands[lands_i].has_barrack = 1;
        lands[lands_i].side = 0;
        
        get_land_coordinates(&x, &y);
        lands[lands_i].barrack_x = x;
        lands[lands_i].barrack_y = y;
        lands[lands_i].barrack_r = 25;
        
        lands[lands_i].soldiers = 12; // TODO change later
        lands[lands_i].max_soldiers = 20;
        lands[lands_i].rebirth_rate = 120;
        lands[lands_i].rebirth_timer = 120;

        lands[lands_i].selected = 0;

        // save land
        save_rand_land(map_file_ptr, map, lands[lands_i].barrack_x, lands[lands_i].barrack_y, lands[lands_i].has_barrack,
                       lands[lands_i].barrack_r, lands[lands_i].side, lands[lands_i].soldiers, lands[lands_i].max_soldiers,
                       lands[lands_i].rebirth_rate, lands[lands_i].rebirth_timer);

        lands_i++;
    }
}

void rand_BFS(int iterator, int map[100][100], int to_check[500][2], int n) {
    if (iterator == 50) return;
    int ss[500][2];
    int l = 0;
    for (int i = 0; i < n; i++) {
        if (to_check[i][0] + 1  < 100 && map[to_check[i][0] + 1][to_check[i][1]] == 0 && (rand()%2 || iterator < rand()%10 + 20)) {
            map[to_check[i][0] + 1][to_check[i][1]] = 1;
            ss[l][0] = to_check[i][0] + 1;
            ss[l][1] = to_check[i][1];
            l++;
        }
        if (to_check[i][0] - 1 >= 0 && map[to_check[i][0] - 1][to_check[i][1]] == 0 && (rand()%2 || iterator < rand()%10 + 20)) {
            map[to_check[i][0] - 1][to_check[i][1]] = 1;
            ss[l][0] = to_check[i][0] - 1;
            ss[l][1] = to_check[i][1];
            l++;
        }
        if (to_check[i][1] + 1 < 100 && map[to_check[i][0]][to_check[i][1] + 1] == 0 && (rand()%2 || iterator < rand()%10 + 20)) {
            map[to_check[i][0]][to_check[i][1] + 1] = 1;
            ss[l][0] = to_check[i][0];
            ss[l][1] = to_check[i][1] + 1;
            l++;
        }
        if (to_check[i][1] - 1 >= 0 && map[to_check[i][0]][to_check[i][1] - 1] == 0 && (rand()%2 || iterator < rand()%10 + 20)) {
            map[to_check[i][0]][to_check[i][1] - 1] = 1;
            ss[l][0] = to_check[i][0];
            ss[l][1] = to_check[i][1] - 1;
            l++;
        }
    }
    rand_BFS(iterator + 1, map, ss, l);
}

void smooth_map(int map[100][100]) {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) { 
            if (map[i][j] == 0) {
                int adjacents = 0;
                if (j > 0 && map[i][j - 1] == 1) adjacents++;
                if (i > 0 && map[i - 1][j] == 1) adjacents++;
                if (j + 1 < 99 && map[i][j + 1] == 1) adjacents++;
                if (i + 1 < 99 && map[i + 1][j] == 1) adjacents++;
                if (i > 0 && j > 0 && map[i - 1][j - 1]) adjacents++;
                if (i > 0 && j < 99 && map[i - 1][j + 1]) adjacents++;
                if (i < 99 && j > 0 && map[i + 1][j - 1]) adjacents++;
                if (i < 99 && j < 99 && map[i + 1][j + 1]) adjacents++;
                if (adjacents > 4) map[i][j] = 1;
            }
        }
    }

}

void save_rand_land(FILE *map_file_ptr, int map[100][100], int x, int y, int has_barrack, int barrack_r,
                    int side, int soldiers, int max_soldiers, int rebirth_rate, int rebirth_timer) {
    fprintf(map_file_ptr, "%d %d %d %d %d %d %d %d %d\n", x, y, has_barrack, barrack_r, side, soldiers,
                                                          max_soldiers, rebirth_rate, rebirth_timer);
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            fprintf(map_file_ptr, "%d ", map[i][j]);
        }
        fprintf(map_file_ptr, "\n");
    }
}

// 0 -> successed, 1 -> get error
int load_rand_map(char file_path[100], int *lands_n, Land lands[]) {
    FILE *file_ptr = fopen(file_path, "r");
    if (file_ptr == NULL) { // check for error
        fprintf(stderr, "%s does not exists.\n", file_path);
        return 1;
    }
    fscanf(file_ptr, "%d", lands_n); // read number



    for (int i = 0; i < *lands_n; i++) { // read lands data
        fscanf(file_ptr, "%hd %hd %d %hd %d %d %d %d %d", &lands[i].barrack_x, &lands[i].barrack_y,
                                                 &lands[i].has_barrack, &lands[i].barrack_r,
                                                 &lands[i].side, &lands[i].soldiers, &lands[i].max_soldiers,
                                                 &lands[i].rebirth_rate, &lands[i].rebirth_timer);
        lands[i].selected = 0;

        for (int m = 0; m < 100; m++) {
            for (int n = 0; n < 100; n++) {
                fscanf(file_ptr, "%d", &lands[i].pixels[m][n]);
            }
        }
    }
    fclose(file_ptr);
    return 0;
}

void apply_rand_map(SDL_Renderer* Renderer, int lands_n, Land lands[], Land* selected_land_ptr) {
    for (int i = 0; i < lands_n; i++) {
        // draw land with 5x5 pixels
        for (int m = 0; m < 100; m++) {
            for (int n = 0; n < 100; n++) {
                if (lands[i].pixels[m][n] == 1) {
                    boxColor(Renderer, lands[i].barrack_x - (50 - m) * 3, lands[i].barrack_y - (50 - n) * 3,
                    lands[i].barrack_x - (50 - m) * 3 + 4, lands[i].barrack_y - (50 - n) * 3 + 4, get_land_normal_color(lands[i].side));
                }
            }
        }
    
    }
    
    for (int i = 0; i < lands_n; i++) {
        if (lands[i].has_barrack) { 
            // draw barrack
            Uint32 color = lands[i].selected && lands[i].side == 1 ? 0xffede2b9: get_side_normal_color(lands[i].side);
            filledCircleColor(Renderer, lands[i].barrack_x, lands[i].barrack_y, lands[i].barrack_r, color);
            
            // add number
            char number[5];
            sprintf(number, "%d", lands[i].soldiers);
            stringRGBA(Renderer, lands[i].barrack_x - 8, lands[i].barrack_y - 5, number, 0, 0, 0, 255);
            
            // rebirth
            if (lands[i].side != 0) { // impartials do not rebirth
                lands[i].rebirth_timer -= 1;
                if (lands[i].rebirth_timer <= 0) {
                    lands[i].rebirth_timer = lands[i].rebirth_rate;
                    if (lands[i].soldiers < lands[i].max_soldiers) {
                        lands[i].soldiers += 1;
                    }
                }
            }
        }
    }
    
    // draw attack line
    draw_attack_line(Renderer, selected_land_ptr);
}

char* get_new_file_name() {
    char* file_name = malloc(50);
    *file_name = '\0';
    
    DIR *maps_dir;
    struct dirent *dir;
    maps_dir = opendir("./data/maps");
    
    int file_number = 0;
    if (maps_dir != NULL) {
        while((dir = readdir(maps_dir)) != NULL) {
            file_number++;
        }
        file_number--; // for . and ..
    }
    else fprintf(stderr, "can not find ./data/maps\n");
    
    char temp[50];
    sprintf(temp, "./data/maps/map%d.txt", file_number);

    strcat(file_name, temp);
    
    return file_name;
}

void get_land_coordinates(int* x, int* y) {
    static int coordinates[20][10] = {{200, 200}, {200, 400}, {200, 600}, {200, 800}, 
                                      {550, 200}, {450, 400}, {550, 600}, {550, 800},
                                      {900, 200}, {900, 400}, {900, 600}, {900, 800},
                                      {1250, 200}, {1250, 400}, {1250, 600}, {1250, 800},
                                      {1600, 200}, {1600, 400}, {1600, 600}, {1600, 800}};
    static int remain = 20;
    
    int target = rand()%remain;
    
    *x = coordinates[target][0] + (rand()%2 ? 1: -1) * rand()%100;
    *y = coordinates[target][1] + (rand()%2 ? 1: -1) * rand()%100;

    // shift
    for (int i = target; i < 19; i++) {
        coordinates[i][0] = coordinates[i + 1][0];
        coordinates[i][1] = coordinates[i + 1][1];
    }
    
    remain--;
    

}


void draw_attack_line(SDL_Renderer* Renderer, Land *selected_land_ptr) {
    if (selected_land_ptr != NULL) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        SDL_RenderDrawLine(Renderer, selected_land_ptr->barrack_x, selected_land_ptr->barrack_y, x, y);
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
        case 6: return 0xff739feb;
        case 7: return 0xff9866e3;
        default: return 0xffcfcdcc;
    }
}

Uint32 get_land_normal_color(int side) {
    switch (side) {
        case 0: return 0xff696966;
        case 1: return 0xff6e3a1a;
        case 2: return 0xff2b2d94;
        case 3: return 0xff479152;
        case 4: return 0xff439c9c;
        case 5: return 0xff753471;
        case 6: return 0xff41629c;
        case 7: return 0xff6c46a6;
        default: return 0xff696966;
    }
}
