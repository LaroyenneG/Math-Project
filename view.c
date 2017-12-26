//
// Created by guillaume on 25/12/17.
//

#include <SDL2/SDL.h>
#include "view.h"
#include "delete-me.h"


static SDL_Window *pWindow = NULL;


SDL_Rect convertRect(point_t a, point_t b) {

    SDL_Rect rect;


    return rect;
}


point_t convertPoint(point_t p) {

    point_t n;


    return n;
}


void showSystem(system_t system) {


    if (pWindow == NULL) {

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            fprintf(stdout, "Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }


        pWindow = SDL_CreateWindow("Pendule avec chariot", SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   1000,
                                   1000,
                                   SDL_WINDOW_SHOWN);


        if (pWindow == NULL) {
            fprintf(stderr, "Erreur de création de la fenêtre: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

    }

    SDL_Renderer *renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "Erreur de création du render: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_RenderClear(renderer);

    SDL_Rect r;
    r.x = 100;
    r.y = 100;
    r.w = 150;
    r.h = 50;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderFillRect(renderer, &r);

    // SDL_RenderDrawLine(SDL_Renderer* renderer, int x1 , int y1 , int x2 , int y2);

    // int SDL_RenderDrawPoint(SDL_Renderer* renderer,int x, int y);


    SDL_RenderPresent(renderer);

    SDL_Delay( 5000 );

}