#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdio.h>
#include "operations.h"

// Init the SDL
int Init_SDL()
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    return 0;
}

SDL_Surface* display_bmp(char *file_name)
{
    SDL_Surface *image;

    /* Load the BMP file into a surface */
    image = SDL_LoadBMP(file_name);
    if (image == NULL) {
        fprintf(stderr, "Couldn't load %s: %s\n", file_name, SDL_GetError());
        return NULL;
    }

    return image;
}