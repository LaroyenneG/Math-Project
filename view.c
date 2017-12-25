//
// Created by guillaume on 25/12/17.
//

#include <SDL2/SDL.h>
#include "view.h"
#include "delete-me.h"


static SDL_Window* pWindow = NULL;

void show(system_t *system, size_t size) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stdout, "Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }


}