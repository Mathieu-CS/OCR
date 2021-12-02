#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include "operations.h"

// -------------------------------------------------------
//                    INTERSECTIONS
// -------------------------------------------------------

// returns the width of the square if square else 0;

int square(int x1, int y1, int x2, int y2)
{
    int w = x2 - x1;
    int h = y2 - y1;
    int diff = w - h;

    if (diff >= -5 && diff <= 5)
    {
        return (w + h) / 2;
    }
    else
    {
        return 0;
    }
}

// return the coordinates of the maximum square found

int __detect_rect(SDL_Surface* image,int x1,int y1)
{
    Uint32 testpixel;
    Uint8 r,g,b;

    int mw = 0;

    for (int j = image->h/2; j < image->h; j++)
    {
        for (int i = image->w/2; i < image->w; i++)
        {
            testpixel = get_pixel(image, i , j);
            SDL_GetRGB(testpixel, image->format, &r,&g,&b);

            if (r == 0 && g == 255 && b == 0) // if intersection
            {
                // square ?
                int temp = square(x1, y1, i, j);

                if (temp > 0)
                {
                    if (temp > mw) // square detected bigger than previous max square
                    {
                        mw = temp;                      
                    }
                }
            }
        }
    }

    return mw;
}

int* detect_rect(SDL_Surface* image)
{
    Uint32 currentpixel;
    Uint8 r,g,b;
    int *p = malloc(3*sizeof(int));


    int rw = 0;
    int rx = 0;
    int ry = 0;

    for (int j = 0; j < image->h/2; j++)
    {
        for (int i = 0; i < image->w/2; i++)
        {
            currentpixel = get_pixel(image, i , j);
            SDL_GetRGB(currentpixel, image->format, &r,&g,&b);

            if (r == 0 && g == 255 && b == 0)
            {
                printf("intersection detected at %i,%i\n", i, j);
                int temp = __detect_rect(image, i, j);
                if (temp > rw)
                {
                    rx = i;
                    ry = j;
                    rw = temp;
                }
                
            }
            
        }
    }
    p[0] = rx;
    p[1] = ry;
    p[2] = rw;
    return p;
}

// -------------------------------------------------------
//                    END OF INTERSECTIONS
// -------------------------------------------------------

// -------------------------------------------------------
//                    PROCESSING
// -------------------------------------------------------

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

            if (r > 100)
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
                if (i - decalagex > 0 && j - decalagey && i - decalagex < 28 && j - decalagey < 28)
                {
                    put_pixel(centered, i - decalagex, j - decalagey, pixel);
                }
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

        return move(image, (int) floor(indicatorx), (int) floor(indicatory));
    }
    else
    {
        return BlackSurface;
    }
}

// cleans the borders of the subimages
void clean(SDL_Surface* image)
{
    int limitw = image->w/7;
    int limith = image->h/7;
    Uint32 white = SDL_MapRGB(image->format, 255, 255, 255);

    // up clean

    for (int i = 0; i < limith; i++)
    {
        for (int j = 0; j < image->w; j++)
        {
            put_pixel(image, j, i, white);
        }
    }
    
    // down clean

    for (int i = image->h - limith; i < image->h; i++)
    {
        for (int j = 0; j < image->w; j++)
        {
            put_pixel(image, j, i, white);
        }
    }

    // left clean

    for (int i = 0; i < image->h; i++)
    {
        for (int j = 0; j < limitw; j++)
        {
            put_pixel(image, j, i, white);
        }
    }
    
    // right clean

    for (int i = 0; i < image->h; i++)
    {
        for (int j = image->w - limitw; j < image->w; j++)
        {
            put_pixel(image, j, i, white);   
        }
    }
}

// splits the image in 81 subimages
void split(SDL_Surface* image, int x0, int y0, int width, int length)
{
    width = (width/9)*9;
    length = width;

    int pathx = width/9;
    int pathy = pathx;

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
            //clean(tosave);
            double zoomx = ((double) 28) / tosave->w;
            double zoomy = ((double) 28) / tosave->h;

            SDL_Surface* totreat = rotozoomSurfaceXY(tosave, 0, zoomx, zoomy, 0);
            
            SDL_SaveBMP(totreat, tot);

            
            /*invert(totreat);
            SDL_Surface* final = center(totreat);

            SDL_SaveBMP(final, tot);

            SDL_FreeSurface(final);
            SDL_FreeSurface(totreat);*/

            k++;

            j = j + pathy;
        }
        j = y0;
        i = i + pathx;
    }
    
    free(splitrect);
    SDL_FreeSurface(tosave);
}

void draw_square(SDL_Surface* image, int x, int y, int w)
{
    Uint32 blue = SDL_MapRGB(image->format, 0, 0, 255);

    for (int i = 0; i < w; i++)
    {
        put_pixel(image, x + i, y, blue);
        put_pixel(image, x + w, y + i, blue);
        put_pixel(image, x + i, y + w, blue);
        put_pixel(image, x, y + i, blue);
    }
    
    SDL_SaveBMP(image, "rect.bmp");
}

int main()  
{
    // argv[1] = edgedetecion.bmp
    // argv[2] = contrast.bmp

    SDL_Surface* hough = display_bmp("../image_preprocessing/edgedetecion.bmp");
    SDL_Surface* contrast = display_bmp("../image_preprocessing/contrast.bmp");

    int* coor = detect_rect(hough);

    printf("image detected\n");
    printf("x = %i\n", coor[0]);
    printf("y = %i\n", coor[1]);
    printf("width = %i\n", coor[2]);
    
    //draw_square(contrast, coor[0], coor[1], coor[2]);
    split(contrast, coor[0], coor[1], coor[2], coor[2]);

    SDL_FreeSurface(hough);
    SDL_FreeSurface(contrast);
    free(coor);
    return 0;
}