#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "operations.h"

void split(SDL_Surface* image, int x0, int y0, int width, int length)
{
    int pathx = width/9;
    int pathy = length/9;

    SDL_Rect* splitrect = malloc(sizeof(SDL_Rect));
    
    splitrect->w = pathx;
    splitrect->h = pathy;
    splitrect->x = x0;
    splitrect->y = y0;

    SDL_Surface* tosave = SDL_CreateRGBSurface(0, splitrect->w, splitrect->h, 32, 0, 0, 0, 0);

    int k = 0;
    char tot[12];

    int i = x0;
    int j = y0;

    while (i < x0 + width)
    {
        while (j < y0 + length)
        {
            splitrect->x = i;
            splitrect->y = j;
            snprintf(tot, 12, "%i.bmp", k);

            SDL_BlitSurface(image, splitrect, tosave, NULL);

            SDL_SaveBMP(tosave, tot);

            k++;

            j = j + pathy;
        }
        k++;
        j = y0;
        i = i + pathx;
    }
    
    free(splitrect);
    SDL_FreeSurface(tosave);
}

int main(int argc,char **argv)  
{
    argc++;

    // argv[1] = image
    // argv[2] = x0
    // argv[3] = y0
    // argv[4] = width
    // argv[5] = height

    SDL_Surface* mysurface = display_bmp(argv[1]);

    split(mysurface, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));

    return 0;

    SDL_FreeSurface(mysurface);
}