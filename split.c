#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdio.h>
#include "operations.h"
#include "split.h"

void name_of_file(char name[], int number)
{
    name[0] = (char) number;
}

void split(char *filename)
{
    Init_SDL(); // Init

    SDL_Surface *origin = SDL_LoadBMP(filename); // Load the file

    SDL_Rect *format = malloc(sizeof(*origin));

    int addx = origin->w/9; // variable we will use to define the Rect position
    int addy = origin->h/9;

    format->x = 0; // rect original properties
    format->y = 0;
    format->w = addx;
    format->h = addy;

    SDL_Surface *destination; // init a Destination Surface
    Uint32 rmask, gmask, bmask, amask;

    if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    {
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    }
    else
    {
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    }    

    destination = SDL_CreateRGBSurface(0, addx, addy, 32, rmask, gmask, bmask, amask);
    if (destination == NULL) {
        printf("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
    }

    int number = 0; // variable for the name of the files we'll save
    char name[] = "x.bmp";

    for (size_t i = 0; i < 9; i++)
    {
        for (size_t j = 0; j < 9; j++)
        {
            format->x = addx * j;

            if (SDL_BlitSurface(origin, format, destination, NULL) != 0)
            {
                // Error splitting the surface
                printf("SDL_BlitSurface failed: %s\n", SDL_GetError());
            }

            //name_of_file(name, number);

            if (SDL_SaveBMP(destination, name) != 0) // save the grid in .bmp format
            {
                // Error saving Bitmap
                printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
            }

            number++;
        }
        
        format->y = addy * i;
    }

    //free the memory
    free(format);
    SDL_FreeSurface(origin);
    SDL_FreeSurface(destination);
}