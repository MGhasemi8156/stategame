#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>


#include "map.h"


void rand_BFS(int iterator, int map[PIXELS_L][PIXELS_L], int to_check[500][2], int n);

void save_rand_land(FILE *map_file_ptr, int map[PIXELS_L][PIXELS_L], int x, int y, int has_barrack, int barrack_r,
                    int side, int soldiers, int max_soldiers, int rebirth_rate, int rebirth_timer);

void take_screenshot(SDL_Renderer* Renderer);

void smooth_map(int map[PIXELS_L][PIXELS_L]);

int get_new_file_number(); 

void get_land_coordinates(int* x, int* y);

Uint32 get_side_normal_color(int side);
Uint32 get_land_normal_color(int side);

void draw_attack_line(SDL_Renderer* Renderer, Land *selected_land_ptr);

int new_file_number = -1;
int image_saved = 0;


void create_rand_map(int lands_n, Land lands[], int players) {
    // open file to save rand map
    new_file_number = get_new_file_number();
    char file_name[50];
    sprintf(file_name, "./data/maps/map%d.txt", new_file_number);
    FILE* map_file_ptr = fopen(file_name, "w");
    
    if (map_file_ptr == NULL) fprintf(stderr, "Can't create %s\n", file_name);
    
    
    // add number of lands to file
    if (map_file_ptr != NULL) fprintf(map_file_ptr, "%d\n", lands_n);
    
    int share = lands_n/(players + 2);
    int lands_i = 0;
    int x, y;
    
    for (int i = 1; i < players + 1; i++) { // i refers to side
        for (int j = 0; j < share; j++) {
            int map[PIXELS_L][PIXELS_L] = {0};
            int to_check[500][2] = {{PIXELS_L/2, PIXELS_L/2}};

            rand_BFS(0, map, to_check, 1);
            smooth_map(map);        
    
            memcpy(lands[lands_i].pixels, map, PIXELS_L * PIXELS_L * sizeof(int));
            
            lands[lands_i].has_barrack = 1;
            lands[lands_i].side = i;
            
            get_land_coordinates(&x, &y);
            lands[lands_i].barrack_x = x;
            lands[lands_i].barrack_y = y;
            lands[lands_i].barrack_r = BARRACK_R;
            
            lands[lands_i].soldiers = 12; // TODO change later
            lands[lands_i].max_soldiers = MAX_SOLDIERS;
            lands[lands_i].rebirth_rate = REBIRTH_RATE;
            lands[lands_i].rebirth_timer = REBIRTH_RATE;
           
            lands[lands_i].selected = 0;
            lands[lands_i].is_attaking = 0;
            lands[lands_i].attack_queue = 0;

            // potions stuff
            lands[lands_i].unlimited_rebirth = 0;
            
            // save land
            if (map_file_ptr != NULL) save_rand_land(map_file_ptr, map, lands[lands_i].barrack_x,
                                                lands[lands_i].barrack_y, lands[lands_i].has_barrack,
                                                lands[lands_i].barrack_r, lands[lands_i].side,
                                                lands[lands_i].soldiers, lands[lands_i].max_soldiers,
                                                lands[lands_i].rebirth_rate, lands[lands_i].rebirth_timer);
            lands_i++;
        }
    }
    
    // lands with no side
    int impartials = lands_n - lands_i;
    for (int i = 0; i < impartials; i++) {
        int map[PIXELS_L][PIXELS_L] = {0};
        int to_check[500][2] = {{PIXELS_L/2, PIXELS_L/2}};       

        rand_BFS(0, map, to_check, 1);
        smooth_map(map);

        memcpy(lands[lands_i].pixels, map, PIXELS_L * PIXELS_L * sizeof(int));
        
        lands[lands_i].has_barrack = 1;
        lands[lands_i].side = 0;
        
        get_land_coordinates(&x, &y);
        lands[lands_i].barrack_x = x;
        lands[lands_i].barrack_y = y;
        lands[lands_i].barrack_r = BARRACK_R;
        
        lands[lands_i].soldiers = 12;
        lands[lands_i].max_soldiers = IMPARTIAL_MAX_SOLDIERS;
        lands[lands_i].rebirth_rate = IMPARTIAL_REBIRTH_RATE;
        lands[lands_i].rebirth_timer = IMPARTIAL_REBIRTH_RATE;

        lands[lands_i].selected = 0;
        lands[lands_i].is_attaking = 0;
        lands[lands_i].attack_queue = 0;

        // potions stuff
        lands[lands_i].unlimited_rebirth = 0;

        // save land
        if (map_file_ptr != NULL) save_rand_land(map_file_ptr, map, lands[lands_i].barrack_x,
                                                 lands[lands_i].barrack_y, lands[lands_i].has_barrack,
                                                 lands[lands_i].barrack_r, lands[lands_i].side,
                                                 lands[lands_i].soldiers, lands[lands_i].max_soldiers,
                                                 lands[lands_i].rebirth_rate, lands[lands_i].rebirth_timer);
        lands_i++;
    }
    
    // close file
    fclose(map_file_ptr);
}

void rand_BFS(int iterator, int map[PIXELS_L][PIXELS_L], int to_check[500][2], int n) {
    if (iterator == 50) return;
    int ss[500][2];
    int l = 0;
    for (int i = 0; i < n; i++) {
        if (to_check[i][0] + 1  < PIXELS_L && map[to_check[i][0] + 1][to_check[i][1]] == 0 && (rand()%2 || iterator < rand()%10 + 20)) {
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
        if (to_check[i][1] + 1 < PIXELS_L && map[to_check[i][0]][to_check[i][1] + 1] == 0 && (rand()%2 || iterator < rand()%10 + 20)) {
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

void smooth_map(int map[PIXELS_L][PIXELS_L]) {
    for (int i = 0; i < PIXELS_L; i++) {
        for (int j = 0; j < PIXELS_L; j++) { 
            if (map[i][j] == 0) {
                int adjacents = 0;
                if (j > 0 && map[i][j - 1] == 1) adjacents++;
                if (i > 0 && map[i - 1][j] == 1) adjacents++;
                if (j + 1 < PIXELS_L && map[i][j + 1] == 1) adjacents++;
                if (i + 1 < PIXELS_L && map[i + 1][j] == 1) adjacents++;
                if (i > 0 && j > 0 && map[i - 1][j - 1]) adjacents++;
                if (i > 0 && j < PIXELS_L && map[i - 1][j + 1]) adjacents++;
                if (i < PIXELS_L && j > 0 && map[i + 1][j - 1]) adjacents++;
                if (i < PIXELS_L && j < PIXELS_L && map[i + 1][j + 1]) adjacents++;
                if (adjacents > 4) map[i][j] = 1;
            }
        }
    }

}

void save_rand_land(FILE *map_file_ptr, int map[PIXELS_L][PIXELS_L], int x, int y, int has_barrack, int barrack_r,
                    int side, int soldiers, int max_soldiers, int rebirth_rate, int rebirth_timer) {
    fprintf(map_file_ptr, "%d %d %d %d %d %d %d %d %d\n", x, y, has_barrack, barrack_r, side, soldiers,
                                                          max_soldiers, rebirth_rate, rebirth_timer);
    for (int i = 0; i < PIXELS_L; i++) {
        for (int j = 0; j < PIXELS_L; j++) {
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
        lands[i].is_attaking = 0;
        lands[i].attack_queue = 0;

        // potions stuff
        lands[i].unlimited_rebirth = 0;

        for (int m = 0; m < PIXELS_L; m++) {
            for (int n = 0; n < PIXELS_L; n++) {
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
        for (int m = 0; m < PIXELS_L; m++) {
            for (int n = 0; n < PIXELS_L; n++) {
                if (lands[i].pixels[m][n] == 1) {
                    boxColor(Renderer, lands[i].barrack_x - (PIXELS_L/2 - m) * PIXELS_F, lands[i].barrack_y - (PIXELS_L/2 - n) * PIXELS_F,
                    lands[i].barrack_x - (PIXELS_L/2 - m) * PIXELS_F + PIXELS_W, lands[i].barrack_y - (PIXELS_L/2 - n) * PIXELS_F + PIXELS_W, get_land_normal_color(lands[i].side));
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
                    if (lands[i].soldiers < lands[i].max_soldiers || lands[i].unlimited_rebirth) {
                        lands[i].soldiers += 1;
                    }
                }
            }
            
            // off is attacking
            if (lands[i].attack_queue <= 0) lands[i].is_attaking = 0;
        }
    }
    
    // take screenshot of map
    if (new_file_number != -1 && !image_saved) {
        take_screenshot(Renderer);    
    }

    // draw attack line
    draw_attack_line(Renderer, selected_land_ptr);
}


// -1 -> got error
int get_new_file_number() {
    DIR *maps_dir;
    struct dirent *dir;
    maps_dir = opendir("./data/maps");
    if (maps_dir == NULL) fprintf(stderr, "Can't open dir ./data/maps\n");
    
    int file_number = 0;
    if (maps_dir != NULL) {
        while((dir = readdir(maps_dir)) != NULL) {
            file_number++;
        }
        file_number--; // for . and ..
    }
    
    return file_number;
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

void take_screenshot(SDL_Renderer* Renderer) {
    char file_name[50];
    sprintf(file_name, "./data/screenshots/map%d.bmp", new_file_number);
    // Get width and height of screen
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    
    SDL_Surface* screenshot = SDL_CreateRGBSurface(0, DM.w, DM.h, 32, 0x00ff0000,
                              0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(Renderer, NULL, SDL_PIXELFORMAT_ARGB8888, screenshot->pixels, screenshot->pitch);
    int r = SDL_SaveBMP(screenshot, file_name);
    SDL_FreeSurface(screenshot);
    
    if (r != 0) fprintf(stderr, "Can't save screenshot at %s\n", file_name);
    else image_saved = 1;
}

void draw_attack_line(SDL_Renderer* Renderer, Land *selected_land_ptr) {
    static int opacity = 100;
    static int direction = 0;
    if (selected_land_ptr != NULL) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        
        double rx = x - selected_land_ptr->barrack_x;
        double ry = y - selected_land_ptr->barrack_y;
                
        double l = sqrt(pow(rx, 2) + pow(ry, 2));
        
        int x1 = selected_land_ptr->barrack_x + ry/l * 7;
        int y1 = selected_land_ptr->barrack_y - rx/l * 7;        
        
        int x2 = selected_land_ptr->barrack_x - ry/l * 7;
        int y2 = selected_land_ptr->barrack_y + rx/l * 7;
        
        filledTrigonRGBA(Renderer, x1, y1, x2, y2, x, y, 187, 255, 0, opacity);
        if (opacity > 250) direction = 1;
        if (opacity < 100) direction = 0;
        if (direction) opacity -= 5;
        else opacity += 5;
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
        default: return 0xffcfcdcc;
    }
}

Uint32 get_land_normal_color(int side) {
    switch (side) {
        case 0: return 0xffd4d4cd;
        case 1: return 0xfffaad7d;
        case 2: return 0xff8587ff;
        case 3: return 0xff9ff5ac;
        case 4: return 0xff96ffff;
        case 5: return 0xffff82f7;
        case 6: return 0xff94bbff;
        default: return 0xff696966;
    }
}
