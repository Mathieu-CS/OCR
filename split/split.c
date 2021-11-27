#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include "operations.h"

// inverts black and white pixels
void invert (SDL_Surface* image)
{
    Uint32 black = SDL_MapRGB(image->format, 0, 0, 0);
    Uint32 white = SDL_MapRGB(image->format, 255, 255, 255);

    Uint32 pixel;
    Uint8 r,g,b;

    for (int i = 0; i < image->w; i++)
    {
        for (int j = 0; j < image->h; j++)
        {
            pixel = get_pixel(image,i,j);
            SDL_GetRGB(pixel, image->format, &r,&g,&b);

            if (r > 200)
            {
                put_pixel(image,i,j,black);
            }
            else
            {
                put_pixel(image,i,j,white);
            }      
        }
    }
    
}

SDL_Surface* move(SDL_Surface* image, int decalagex, int decalagey)
{
    Uint32 pixel;
    Uint8 r1,g1,b1;
    SDL_Surface* centered = SDL_CreateRGBSurface(0,28,28,32,0,0,0,0);

    for (int i = 0; i < image->w; i++)
    {
        for (int j = 0; j < image->h; j++)
        {
            pixel = get_pixel(image,i,j);
            SDL_GetRGB(pixel, image->format, &r1,&g1,&b1);

            if (r1 == 255)
            {
                put_pixel(centered, i - decalagex, j - decalagey, pixel);
            }
        }
    }

    return centered;
}

// centers the number in the subimage
SDL_Surface* center(SDL_Surface* image)
{
    int totx = 0;
    int toty = 0;
    int numberpixel = 0;
    SDL_Surface* BlackSurface = SDL_CreateRGBSurface(0,28,28,32,0,0,0,0);
    
    Uint32 pixel;
    Uint8 r,g,b;

    for (int i = 0; i < image->w; i++)
    {
        for (int j = 0; j < image->h; j++)
        {
            pixel = get_pixel(image,i,j);
            SDL_GetRGB(pixel, image->format, &r,&g,&b);

            if (r == 255)
            {
                totx += i;
                toty += j;
                numberpixel++;   
            }
        }
    }

    if (numberpixel != 0)
    {
        float indicatorx = ((float)totx/numberpixel) - 14;
        float indicatory = ((float)toty/numberpixel) - 14;

        printf("décalage des x : %i\n", (int) ceil(indicatorx));
        printf("décalage des y : %i\n", (int) ceil(indicatory));

        return move(image, (int) floor(indicatorx), (int) floor(indicatory));
    }
    else
    {
        return BlackSurface;
    }
}

// cleans the borders of the subimages
void clean(SDL_Surface* image, int width, int height)
{
    int limitw = width/5;
    int limith = height/5;
    Uint32 white = SDL_MapRGB(image->format, 255, 255, 255);

    // up clean

    for (int i = 0; i < limith; i++)
    {
        for (int j = 0; j < width; j++)
        {
            put_pixel(image, j, i, white);
        }
    }
    
    // down clean

    for (int i = height - limith; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            put_pixel(image, j, i, white);
        }
    }

    // left clean

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < limitw; j++)
        {
            put_pixel(image, j, i, white);
        }
    }
    
    // right clean

    for (int i = 0; i < height; i++)
    {
        for (int j = width - limitw; j < width; j++)
        {
            put_pixel(image, j, i, white);   
        }
    }
}

// splits the image in 81 subimages
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
    char tot[15];

    int i = x0;
    int j = y0;

    while (i < x0 + width)
    {
        while (j < y0 + length)
        {
            splitrect->x = i;
            splitrect->y = j;
            snprintf(tot, 15, "%i.bmp", k);

            SDL_BlitSurface(image, splitrect, tosave, NULL);
            clean(tosave, tosave->w, tosave->h);

            double zoomx = ((double) 28) / tosave->w;
            double zoomy = ((double) 28) / tosave->h;

            SDL_Surface* totreat = rotozoomSurfaceXY(tosave, 0, zoomx, zoomy, 0);
            invert(totreat);
            SDL_Surface* final = center(totreat);

            SDL_SaveBMP(final, tot);

            SDL_FreeSurface(final);
            SDL_FreeSurface(totreat);

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
