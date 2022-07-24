#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "reconstruction.h"
#include "../image_preprocessing/operations.h"
#include "SDL/SDL_rotozoom.h"

void drawoutlines(SDL_Surface* image)
{
    Uint32 black = SDL_MapRGB(image->format, 0,0,0);

    for (int i = 0; i < image->w; i++)
    {
        put_pixel(image,i,0,black);
        put_pixel(image,i, image->h - 1, black);
    }

    for (int i = 0; i < image->h; i++)
    {
        put_pixel(image,0, i, black);
        put_pixel(image,image->w - 1, i, black);
    }
}

void drawfatlines(SDL_Surface* image,int x,int y,int width,int height)
{
    Uint32 black = SDL_MapRGB(image->format, 0,0,0);
    int firstline = width/3;
    int secondline = firstline * 2;

    for (int i = firstline - 2; i < firstline + 3; i++)
    {
        for (int j = y; j <= height; j++)
        {
            put_pixel(image, i, j, black);
        }
    }

    for (int i = secondline - 2; i < secondline + 3; i++)
    {
        for (int j = y; j <= height; j++)
        {
            put_pixel(image, i, j, black);
        }   
    }

    int thirdline = height/3;
    int fourthline = thirdline*2;

    for (int i = x; i <= width; i++)
    {
        for (int j = thirdline - 1; j < thirdline + 2; j++)
        {
            put_pixel(image, i, j, black);
        }
    }
    
    for (int i = x; i <= width; i++)
    {
        for (int j = fourthline - 1; j < fourthline + 2; j++)
        {
            put_pixel(image, i, j, black);
        }
    }
}

void pastesurface(SDL_Surface* image, SDL_Surface* topaste, int x, int y, float format)
{
    SDL_Surface* scaled = rotozoomSurfaceXY(topaste, 0, format/topaste->w, format/topaste->h, 0);

    for (int i = 0; i < scaled->w; i++)
    {
        for (int j = 0; j < scaled->h; j++)
        {
            Uint32 pixel = get_pixel(scaled, i, j);
            put_pixel(image, i + x, j + y, pixel);
        }
    }
    SDL_FreeSurface(scaled);
}

SDL_Surface* getsurfacefromint(int x)
{
    SDL_Surface* mysurface;

    switch (x)
    {
    case 1:
        mysurface = display_bmp("rec_images/1.bmp");
        break;
    case 2:
        mysurface = display_bmp("rec_images/2.bmp");
        break;
    case 3:
        mysurface = display_bmp("rec_images/3.bmp");
        break;
    case 4:
        mysurface = display_bmp("rec_images/4.bmp");
        break;
    case 5:
        mysurface = display_bmp("rec_images/5.bmp");
        break;
    case 6:
        mysurface = display_bmp("rec_images/6.bmp");
        break;
    case 7:
        mysurface = display_bmp("rec_images/7.bmp");
        break;
    case 8:
        mysurface = display_bmp("rec_images/8.bmp");
        break;
    case 9:
        mysurface = display_bmp("rec_images/9.bmp");
        break;
    
    default:
        mysurface = display_bmp("rec_images/1.bmp");
        break;
    }

    drawoutlines(mysurface);
    return mysurface;
}

SDL_Surface* getsurfacefromintred(int x)
{
    SDL_Surface* mysurface;

    switch (x)
    {
    case 1:
        mysurface = display_bmp("rec_images/11.bmp");
        break;
    case 2:
        mysurface = display_bmp("rec_images/12.bmp");
        break;
    case 3:
        mysurface = display_bmp("rec_images/13.bmp");
        break;
    case 4:
        mysurface = display_bmp("rec_images/14.bmp");
        break;
    case 5:
        mysurface = display_bmp("rec_images/15.bmp");
        break;
    case 6:
        mysurface = display_bmp("rec_images/16.bmp");
        break;
    case 7:
        mysurface = display_bmp("rec_images/17.bmp");
        break;
    case 8:
        mysurface = display_bmp("rec_images/18.bmp");
        break;
    case 9:
        mysurface = display_bmp("rec_images/19.bmp");
        break;
    
    default:
        mysurface = display_bmp("rec_images/11.bmp");
        break;
    }

    drawoutlines(mysurface);
    return mysurface;
}


void reconstruction(char* solved, char* based)
{
    // method to read a file found at stackoverflow.com/questions/3501338/c-read-file-line-by-line
    // potential problem as the function getline is specific to GNU environement

    SDL_Surface* baseimage = SDL_CreateRGBSurface(0, 1000, 1000, 32, 0, 0, 0, 0);

    // step 2 : extract infos from solved file :

    int solvedgrid[9][9];
    
    FILE *fh = fopen(solved, "r");

    if (fh == NULL)
    {
        perror("Error while opening the solved grid file.\n");
        exit(EXIT_FAILURE);
    }

    int ix = 0;
    int iy = 0;
    char ch;

    while ((ch = fgetc(fh)) != EOF)
    {
        if (ch != ' ' && ch != '\n')
        {
            solvedgrid[ix][iy] = ((int) ch) - 48;
            ix++;

            if (ix == 9)
            {
                iy++;
                ix = 0;
            }
        }
    }

    fclose(fh);

    // end of step 2

    // step 2.5 extract infos from

    int basedsolvedgrid[9][9];
    
    FILE *fg = fopen(based, "r");

    if (fg == NULL)
    {
        perror("Error while opening the solved grid file.\n");
        exit(EXIT_FAILURE);
    }

    ix = 0;
    iy = 0;
    char cg;

    while ((cg = fgetc(fg)) != EOF)
    {
        if (((int) cg >= 48 && (int) cg <= 57) || (int) cg == 46)
        {
            basedsolvedgrid[ix][iy] = ((int) cg) - 48;
            ix++;

            if (ix == 9)
            {
                iy++;
                ix = 0;
            }
        }
    }

    fclose(fg);

    // step 3 : reconstruction of the grid

    int x = 0;
    int y = 0;
    int width = 999;
    int height = 999;

    int stepw = width/9;
    int steph = height/9;
    float format = width / 9;
    int number;
    int a = 0;
    int b = 0;

    for (int j = y; j < height; j += steph)
    {
        for (int i = x; i < width; i += stepw)
        {
            SDL_Surface* topaste;
            number = solvedgrid[a][b];

            if (basedsolvedgrid[a][b] == -2)
            {
                topaste = getsurfacefromintred(number); // new
            }
            else
            {
                topaste = getsurfacefromint(number); // already on the image
            }
            pastesurface(baseimage, topaste, i, j, format);
            SDL_FreeSurface(topaste);
            a++;
        }
        a = 0;
        b++;
    }

    drawfatlines(baseimage, x, y, width, height);

    if (SDL_SaveBMP(baseimage, "end.bmp") != 0)
    {
        //Error saving bitmap
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(baseimage);
}
