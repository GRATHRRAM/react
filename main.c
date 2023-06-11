#define SDL_MAIN_HANDLED
#include "src\include\SDL.h"
#include "src\include\SDL_ttf.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#define true 1
#define false 0
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 750
//gcc -Isrc\include -Lsrc\libs -o out.exe main.c  -lSDL2main -lSDL2 -mwindows -lSDL2_ttf


typedef struct color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} color;


typedef struct gameinfo
{
    double max_rect;
    uint64_t max_rect_inc;
    uint64_t level;
    SDL_FRect *rects;
    uint8_t *dirs;
    color *colors;
    float SPEED;
    float SPEED_INC;
    double miss;
} gameinfo;

uint64_t calk_bt(uint64_t max_rect);
void new_rect(gameinfo *gmi, uint64_t mode);
uint8_t errcheck(SDL_Window *win,SDL_Renderer *ren);

//defineing staff

int main(void)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *calibri = NULL;
    SDL_Color font_color;
    SDL_Event event;
    gameinfo gmi;
    uint64_t gone_cout = 0;
    SDL_Color bgc = {100, 149, 237, 255};
    gmi.level = 0;
    char buff[1000];
    char text[1000];
    srand(time(NULL));

    window = SDL_CreateWindow("REACT!", 
			    SDL_WINDOWPOS_UNDEFINED, 
			    SDL_WINDOWPOS_UNDEFINED, 
			    WINDOW_WIDTH, 
			    WINDOW_HEIGHT, 
			    0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    calibri = TTF_OpenFont("calibri.ttf", 24);

    font_color.a = 255;
    font_color.r = 255;
    font_color.g = 0;
    font_color.b = 0;
    

    SDL_Surface* smess =
    TTF_RenderText_Solid(calibri, "Level -> 0; last prc -> 0.0", font_color);
    SDL_Texture* tmess = SDL_CreateTextureFromSurface(renderer, smess); 
    SDL_Rect rmess;
    rmess.h = 100;
    rmess.w = 1200;
    rmess.x = 0;
    rmess.y = 0;


    gmi.SPEED = 0.1;
    gmi.SPEED_INC = 0.1;
    gmi.max_rect = 5;
    gmi.max_rect_inc = 5;

    printf("\nAlocating %d bites; %d kb; %d mb...", calk_bt(gmi.max_rect),
    calk_bt(gmi.max_rect) / 1024,
    (calk_bt(gmi.max_rect) / 1024) / 1024);


    gmi.rects = (SDL_FRect*) malloc(sizeof(SDL_FRect) * gmi.max_rect);
    gmi.dirs = (uint8_t*) malloc(sizeof(uint8_t) * gmi.max_rect);
    gmi.colors = (color*) malloc(sizeof(color) * gmi.max_rect);

    if(gmi.rects == NULL || gmi.dirs == NULL)
    {
        printf("Cant alloc %d bites; %d kb; %d mb\n", calk_bt(gmi.max_rect),
        calk_bt(gmi.max_rect) / 1024,
        (calk_bt(gmi.max_rect) / 1024) / 1024);
        return 4;
    }

    //errorcheck
    uint8_t err = errcheck(window, renderer);
    if(err != 0) return err;
    //errcheck

    //allocating && setting staff

    printf("done!\n");
    new_rect(&gmi, 0);
    printf("preparing rects Done!\n");

    //game loop start
    while (true)
    {
        gone_cout = 0;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                break;
            }
            if(event.type == SDL_KEYUP)
            {
                if(event.key.keysym.sym == SDLK_ESCAPE) break;
                if(event.key.keysym.sym == SDLK_r) new_rect(&gmi, 0);
                if(event.key.keysym.sym == SDLK_c) {
                    bgc.r = rand() % 255;
                    bgc.g = rand() % 255;
                    bgc.b = rand() % 255;
                }
                if(event.key.keysym.sym == SDLK_v) {
                    font_color.r = rand() % 255;
                    font_color.g = rand() % 255;
                    font_color.b = rand() % 255;

                    memset(buff,0,sizeof(buff));
                    memset(text,0,sizeof(text));
                    sprintf(buff, "%f", gmi.miss);
                    strcat(text, "level -> ");
                    strcat(text, buff);
                    memset(buff,0,sizeof(buff));
                    strcat(text, " ;last level prc -> ");

                    memset(buff,0,sizeof(buff));
                    sprintf(buff , "%.2f%%", (double) ((gmi.max_rect / (gmi.max_rect + gmi.miss + 0.0))) * 100.0);
                    strcat(text, buff);


                    smess = TTF_RenderText_Solid(calibri, text, font_color);
                    tmess = SDL_CreateTextureFromSurface(renderer, smess);
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    for(uint64_t cout = 0; cout < gmi.max_rect; cout++)
                    {
                        Sint32 mouse_x = (Sint32) event.motion.x;
                        Sint32 mouse_y = (Sint32) event.motion.y;
                        if(mouse_x >= gmi.rects[cout].x && mouse_x <= gmi.rects[cout].x + gmi.rects[cout].w &&
                        mouse_y >= gmi.rects[cout].y && mouse_y <= gmi.rects[cout].y + gmi.rects[cout].h) {
                            gmi.rects[cout].h = 0;
                        }
                        else gmi.miss++;
                    
                    }
                }
            }
        }
        //end of input handeling

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,tmess, NULL, &rmess);

        for(uint64_t cout = 0; cout < gmi.max_rect; cout++)
        {
            if(gmi.rects[cout].h != 0)
            {
                SDL_SetRenderDrawColor(renderer, gmi.colors[cout].red, gmi.colors[cout].green, gmi.colors[cout].blue, 255);
                SDL_RenderFillRectF(renderer, &gmi.rects[cout]);

                if(rand() % 100000 <= 10) gmi.dirs[cout] = rand() % 7;

                if(gmi.dirs[cout] == 0) gmi.rects[cout].y -= 0.1f * gmi.SPEED;//up
                if(gmi.dirs[cout] == 1) gmi.rects[cout].y += 0.1f * gmi.SPEED;//dwn
                if(gmi.dirs[cout] == 2) gmi.rects[cout].x -= 0.1f * gmi.SPEED;//lft
                if(gmi.dirs[cout] == 3) gmi.rects[cout].x += 0.1f * gmi.SPEED;//rht
                if(gmi.dirs[cout] == 4) {//lewy gura
                    gmi.rects[cout].y -= 0.07f * gmi.SPEED;
                    gmi.rects[cout].x -= 0.07f * gmi.SPEED;
                }
                if(gmi.dirs[cout] == 5) {//prawy gura
                    gmi.rects[cout].y -= 0.07f * gmi.SPEED;
                    gmi.rects[cout].x += 0.07f * gmi.SPEED;
                }
                if(gmi.dirs[cout] == 6) {//lewy dul
                    gmi.rects[cout].y += 0.07f * gmi.SPEED;
                    gmi.rects[cout].x -= 0.07f * gmi.SPEED;
                }
                if(gmi.dirs[cout] == 7) {//prawy dul
                    gmi.rects[cout].y += 0.07f * gmi.SPEED;
                    gmi.rects[cout].x += 0.07f * gmi.SPEED;
                }

                if(gmi.rects[cout].x + gmi.rects[cout].w >= WINDOW_WIDTH) {
                    int rnd = rand() % 2;
                    if(rnd == 0) gmi.dirs[cout] = 2;
                    else if(rnd == 1) gmi.dirs[cout] = 4;
                    else gmi.dirs[cout] = 6;
                }
                if(gmi.rects[cout].y + gmi.rects[cout].h >= WINDOW_HEIGHT) { 
                    int rnd = rand() % 2;
                    if(rnd == 0) gmi.dirs[cout] = 0;
                    else if(rnd == 1) gmi.dirs[cout] = 5;
                    else gmi.dirs[cout] = 4;
                }
                if(gmi.rects[cout].x <= 0) {
                    int rnd = rand() % 2;
                    if(rnd == 0) gmi.dirs[cout] = 3;
                    else if(rnd == 1) gmi.dirs[cout] = 5;
                    else gmi.dirs[cout] = 7;
                }
                if(gmi.rects[cout].y <= 0) {
                    int rnd = rand() % 2;
                    if(rnd == 0) gmi.dirs[cout] = 1;
                    else if(rnd == 1) gmi.dirs[cout] = 6;
                    else gmi.dirs[cout] = 7;
                }
            }
            else gone_cout++;

            if(gone_cout == gmi.max_rect)
            {
                gmi.level++;
                memset(buff,0,sizeof(buff));
                memset(text,0,sizeof(text));
                sprintf(buff, "%d", gmi.level);
                strcat(text, "level -> ");
                strcat(text, buff);
                strcat(text, "  last level -> ");

                memset(buff,0,sizeof(buff));
                sprintf(buff , "%.2f%%", (double) ((gmi.max_rect / (gmi.max_rect + gmi.miss + 0.0))) * 100.0);
                strcat(text, buff);
                gmi.miss = 0;

                smess = TTF_RenderText_Solid(calibri, text, font_color);
                tmess = SDL_CreateTextureFromSurface(renderer, smess);

                if(gmi.SPEED >! 350) gmi.SPEED += gmi.SPEED_INC;
                gmi.max_rect += gmi.max_rect_inc;
                gmi.rects = (SDL_FRect*) realloc(gmi.rects, sizeof(SDL_FRect) * gmi.max_rect);
                gmi.colors = (color*) realloc(gmi.colors, sizeof(color) * gmi.max_rect);
                gmi.dirs = (uint8_t*) realloc(gmi.dirs, sizeof(uint8_t) * gmi.max_rect);
                new_rect(&gmi,0);
            }

            //rendering and moving rects

        }

        //other staff
        SDL_SetRenderDrawColor(renderer, bgc.r, bgc.g, bgc.b, 255);
        SDL_RenderPresent(renderer);
    }
  

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit(); 
  
    return 0;
}
//Oldbik#0869

uint64_t calk_bt(uint64_t max_rect)
{
    return (sizeof(SDL_FRect) * max_rect) + (sizeof(uint8_t) * max_rect) + (sizeof(color) * max_rect);
}

void new_rect(gameinfo *gmi, uint64_t mode)
{
    if(mode == 0)
    {
        for(uint64_t cout = 0; cout < gmi->max_rect; cout++)
        {
            uint8_t gen = (rand() % 220) + 35;
            gmi->rects[cout].h = gen;
            gmi->rects[cout].w = gen;        
            gmi->rects[cout].x = rand() % WINDOW_WIDTH; 
            gmi->rects[cout].y = rand() % WINDOW_HEIGHT;  
            gmi->colors[cout].red = rand() % 255;
            gmi->colors[cout].green = rand() % 255;
            gmi->colors[cout].blue = rand() % 255;
            gmi->dirs[cout] = rand() % 7;

            if(gmi->colors[cout].blue > 140 && gmi->colors[cout].blue < 153) gmi->colors[cout].blue += 11;
        }
    }
    else 
    {
        uint8_t gen = (rand() % 235) + 20;
        gmi->rects[mode].h = gen;
        gmi->rects[mode].w = gen;        
        gmi->rects[mode].x = rand() % WINDOW_WIDTH; 
        gmi->rects[mode].y = rand() % WINDOW_HEIGHT;  
        gmi->colors[mode].red = rand() % 255;
        gmi->colors[mode].green = rand() % 255;
        gmi->colors[mode].blue = rand() % 255;
        gmi->dirs[mode] = rand() % 7;
    }
}

uint8_t errcheck(SDL_Window *win,SDL_Renderer *ren)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL failed to initialise: %s\n", SDL_GetError());
        return 1;
    }

    if (win == NULL) {
        fprintf(stderr, "SDL window failed to initialise: %s\n", SDL_GetError());
        return 2;
    }

    if (ren == NULL)
    {
        SDL_Log("Could not create a renderer: %s", SDL_GetError());
        return 3;
    } 

    return 0;
}