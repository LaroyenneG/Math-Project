//
// Created by guillaume on 25/12/17.
//

#include <SDL2/SDL.h>
#include "view.h"
#include "delete-me.h"


static SDL_Window *pWindow = NULL;


SDL_Rect convertRect(point_t a, point_t b) {

    SDL_Rect rect;
    rect.x = (int) (a.x * C_MULT) + WINDOWS_WIDTH / 2;
    rect.y = (int) (a.y * C_MULT) + OFFSET_HEIGHT;
    rect.w = (int) ((b.x - a.x) * C_MULT);
    rect.h = (int) ((b.y - a.y) * C_MULT);


    return rect;
}


SDL_Point convertPoint(point_t p) {

    SDL_Point n;

    n.x = (int) (p.x * C_MULT) + WINDOWS_WIDTH / 2;
    n.y = (int) (p.y * C_MULT) + OFFSET_HEIGHT;

    return n;
}


void SDL_Circle(SDL_Renderer *renderer, int cx, int cy, int rayon) {

    if (rayon <= 0) {
        return;
    }

    int y, x;

    double d = 3 - (2 * rayon);
    x = 0;
    y = rayon;

    while (y >= x) {

        SDL_RenderDrawLine(renderer, cx, cy, cx + x, cy + y);
        SDL_RenderDrawLine(renderer, cx, cy, cx + y, cy + x);
        SDL_RenderDrawLine(renderer, cx, cy, cx - x, cy + y);
        SDL_RenderDrawLine(renderer, cx, cy, cx - y, cy + x);
        SDL_RenderDrawLine(renderer, cx, cy, cx + x, cy - y);
        SDL_RenderDrawLine(renderer, cx, cy, cx + y, cy - x);
        SDL_RenderDrawLine(renderer, cx, cy, cx - x, cy - y);
        SDL_RenderDrawLine(renderer, cx, cy, cx - y, cy - x);

        if (d < 0)
            d = d + (4 * x) + 6;
        else {
            d = d + 4 * (x - y) + 10;
            y--;
        }

        x++;
    }
    SDL_Circle(renderer, cx, cy, rayon - 1);
}

void showSystemZero() {

    system_t system = buildSystem(0.0, 0.0);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stdout, "Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }


    SDL_Window *pWindow = SDL_CreateWindow("Pendule avec chariot sans energie", SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED,
                                           WINDOWS_WIDTH,
                                           WINDOWS_HEIGHT,
                                           SDL_WINDOW_SHOWN);


    if (pWindow == NULL) {
        fprintf(stderr, "Erreur de création de la fenêtre: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }


    SDL_Renderer *renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "Erreur de création du render: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }


    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect rectBar = convertRect(system.bar.a, system.bar.b);
    SDL_Rect rectWeight = convertRect(system.weight.a, system.weight.b);

    SDL_RenderFillRect(renderer, &rectBar);
    SDL_RenderFillRect(renderer, &rectWeight);

    SDL_Rect rectTrolley;

    SDL_Point pointTrolley = convertPoint(system.trolley.position);

    rectTrolley.x = pointTrolley.x - 100;
    rectTrolley.y = pointTrolley.y - OFFSET_HEIGHT;
    rectTrolley.h = OFFSET_HEIGHT;
    rectTrolley.w = 200;

    SDL_RenderFillRect(renderer, &rectTrolley);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Point pointPivot = convertPoint(system.pivot.position);
    SDL_RenderDrawPoint(renderer, pointPivot.x, pointPivot.y);
    SDL_Circle(renderer, pointPivot.x, pointPivot.y, 2);


    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    SDL_RenderDrawLine(renderer, 0, OFFSET_HEIGHT, WINDOWS_WIDTH, OFFSET_HEIGHT);


    SDL_SetRenderDrawColor(renderer, 254, 51, 0, 255);
    SDL_Point g0 = convertPoint(system.pendulumGravityCenter);
    SDL_RenderDrawPoint(renderer, g0.x, g0.y);

    SDL_Circle(renderer, g0.x, g0.y, 10);

    printf("Point rouge : (%d, %d) centre de gravité du balancier\n", g0.x, g0.y);


    SDL_SetRenderDrawColor(renderer, 90, 51, 0, 255);
    SDL_Point g1 = convertPoint(system.bar.gravityCenter);
    SDL_RenderDrawPoint(renderer, g1.x, g1.y);

    SDL_Circle(renderer, g1.x, g1.y, 10);

    printf("Point brun : (%d, %d) centre de gravité de la barre\n", g1.x, g1.y);


    SDL_SetRenderDrawColor(renderer, 240, 240, 5, 255);
    SDL_Point g2 = convertPoint(system.weight.gravityCenter);
    SDL_RenderDrawPoint(renderer, g2.x, g2.y);

    SDL_Circle(renderer, g2.x, g2.y, 10);

    printf("Point jaune : (%d, %d) centre de gravité de la masselotte\n", g2.x, g2.y);

    SDL_SetRenderDrawColor(renderer, 10, 151, 40, 255);
    SDL_Point g3 = convertPoint(system.inertiaCenter);
    SDL_RenderDrawPoint(renderer, g3.x, g3.y);

    SDL_Circle(renderer, g3.x, g3.y, 10);

    printf("Point vert : (%d, %d) centre centre d'inertie du système\n", g3.x, g3.y);


    SDL_RenderPresent(renderer);


    SDL_Delay(50000);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void showSystemTime(system_t system) {

    if (pWindow == NULL) {

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            fprintf(stdout, "Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }


        pWindow = SDL_CreateWindow("Pendule avec chariot, à l'instant t", SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   WINDOWS_WIDTH,
                                   WINDOWS_HEIGHT,
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


    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    SDL_RenderDrawLine(renderer, 0, OFFSET_HEIGHT, WINDOWS_WIDTH, OFFSET_HEIGHT);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


    SDL_Rect rectTrolley;
    SDL_Point pointTrolley = convertPoint(system.trolley.position);
    rectTrolley.x = pointTrolley.x - 100;
    rectTrolley.y = pointTrolley.y - OFFSET_HEIGHT;
    rectTrolley.h = OFFSET_HEIGHT;
    rectTrolley.w = 200;

    SDL_RenderFillRect(renderer, &rectTrolley);


    SDL_SetRenderDrawColor(renderer, 20, 120, 20, 255);
    SDL_Point pointPivot = convertPoint(system.pivot.position);
    SDL_RenderDrawPoint(renderer, pointPivot.x, pointPivot.y);
    SDL_Circle(renderer, pointPivot.x, pointPivot.y, 2);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


    SDL_Point l = convertPoint(system.weight.gravityCenter);

    SDL_RenderDrawLine(renderer, pointPivot.x, pointPivot.y, l.x, l.y);


    SDL_Point gWeight = convertPoint(system.weight.gravityCenter);
    SDL_RenderDrawPoint(renderer, gWeight.x, gWeight.y);

    SDL_Circle(renderer, gWeight.x, gWeight.y, 15);


    SDL_SetRenderDrawColor(renderer, 254, 51, 0, 255);
    SDL_Point g0 = convertPoint(system.pendulumGravityCenter);
    SDL_RenderDrawPoint(renderer, g0.x, g0.y);

    SDL_Circle(renderer, g0.x, g0.y, 10);



    SDL_SetRenderDrawColor(renderer, 90, 51, 0, 255);
    SDL_Point g1 = convertPoint(system.bar.gravityCenter);
    SDL_RenderDrawPoint(renderer, g1.x, g1.y);

    SDL_Circle(renderer, g1.x, g1.y, 10);



    SDL_SetRenderDrawColor(renderer, 240, 240, 5, 255);
    SDL_Point g2 = convertPoint(system.weight.gravityCenter);
    SDL_RenderDrawPoint(renderer, g2.x, g2.y);

    SDL_Circle(renderer, g2.x, g2.y, 10);


    SDL_SetRenderDrawColor(renderer, 10, 151, 40, 255);
    SDL_Point g3 = convertPoint(system.inertiaCenter);
    SDL_RenderDrawPoint(renderer, g3.x, g3.y);

    SDL_Circle(renderer, g3.x, g3.y, 10);



    SDL_RenderDrawLine(renderer, g3.x, 0, g3.x, WINDOWS_HEIGHT);


    SDL_RenderPresent(renderer);


    printf("\nInformations sur les energies :\n");
    printf("\tEnergie mécanique=%lf\n", system.mechanicalEnergy);
    printf("\tEnergie cinétique=%lf\n", system.kineticEnergy);
    printf("\tEnergie potentielle=%lf\n", system.potentialEnergy);

    SDL_Delay(1);

    SDL_DestroyRenderer(renderer);
}

