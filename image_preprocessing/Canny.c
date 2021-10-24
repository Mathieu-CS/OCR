#include <err.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "operations.h"
#include "Canny.h"


void RemoveLowPixels(SDL_Surface* image)
{
    Uint32 pixel, black;
    Uint8 r;

    black = SDL_MapRGB(image->format, 0, 0, 0);

    for (int i = 1; i < image->w - 2; i++)
    {
        for (int j = 1; j < image->h - 2; j++)
        {
            pixel = get_pixel(image, i ,j);
            SDL_GetRGB(pixel, image->format, &r,&r,&r);

            if (r < 50)
            {
                put_pixel(image, i, j, black);
            }
        }
    }
}

int GetAverageGradient(SDL_Surface* image)
{
    Uint32 pixel;
    Uint8 r;
    long long tot = 0;
    long count = 0;

    for (int i = 1; i < image->w - 1; i++)
    {
        for (int j = 1; j < image->h - 1; j++)
        {
            pixel = get_pixel(image, i, j);
            SDL_GetRGB(pixel, image->format, &r,&r,&r);

            if (r > 0)
            {
                tot += r;
                count++;
            }
        }
    }

    if (tot < 0 || count < 0)
    {
        printf("GetAverageGradient: overflow");
    }
    
    if (count == 0)
    {
        return 0;
    }
    
    return tot/count;
}

// Gradient Magnitude Tresholding
// https://en.wikipedia.org/wiki/
// Canny_edge_detector#Gradient_magnitude_thresholding_or_lower_bound_cut-off_suppression
void RMS(SDL_Surface* image, int x , int y, int degree)
{
    Uint32 currentpixel, pixel1, pixel2;
    Uint32 black = SDL_MapRGB(image->format, 0,0,0);

    currentpixel = get_pixel(image, x, y);

    //pixel1 = get_pixel(image, x + 1, y + 1); // north east
    //pixel2 = get_pixel(image, x - 1, y + 1); // south west
    //pixel1 = get_pixel(image, x - 1, y - 1); // north west
    //pixel2 = get_pixel(image, x + 1, y - 1); // south east 

    switch (degree)
    {
    case 0:
        pixel1 = get_pixel(image, x - 1, y); // west
        pixel2 = get_pixel(image, x + 1, y); // east
        break;
    case 45:
        pixel1 = get_pixel(image, x - 1, y - 1); // north west
        pixel2 = get_pixel(image, x + 1, y - 1); // south east  
        break;
    case 90:
        pixel1 = get_pixel(image, x, y - 1); // north
        pixel2 = get_pixel(image, x, y + 1); // south
        break;
    case 135:
        pixel1 = get_pixel(image, x + 1, y + 1); // north east
        pixel2 = get_pixel(image, x - 1, y + 1); // south west
        break;
    default:
        pixel1 = get_pixel(image, x - 1, y); // west
        pixel2 = get_pixel(image, x + 1, y); // east
        break;
    }

    Uint8 r1, r2, rr; // get the values of the current pixel and its neighbors

    SDL_GetRGB(currentpixel, image->format, &rr, &rr, &rr);
    SDL_GetRGB(pixel1, image->format, &r1, &r1, &r1);
    SDL_GetRGB(pixel2, image->format, &r2, &r2, &r2);

    if (rr > r1 && rr > r2) // edge detected -> keep
    {
        put_pixel(image, x, y, currentpixel);
    }
    else
    {
        put_pixel(image, x, y, black);
    }
    // else -> remove it (do nothing in this code as we use another surface to draw)
}

// Apply the GMT over the whole image
void NonMaximumEdgeSuppression(SDL_Surface* image, double** M)
{
    printf("Canny : applying gradient magnitude thresholding...\n");
    //SDL_Surface * destination = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);

    for (int i = 1; i < image->w - 2; i++)
    {
        for (int j = 1; j < image->h - 2; j++)
        {
            RMS(image, i, j, M[i][j]);
        }
    }

    if (SDL_SaveBMP(image, "Suppressed.bmp") != 0) // save the grid in .bmp format
    {
        // Error saving Bitmap
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }
}

// check if one of the neighbors is > upperthreshold
bool Keep(SDL_Surface* image, int x, int y, int upperthreshold)
{
    Uint32 pixel_1, pixel_2, pixel_3, pixel_4, pixel_5, pixel_6, pixel_7, pixel_8; 
    
    pixel_1 = get_pixel(image, x - 1, y -1); // first row
    pixel_2 = get_pixel(image, x, y - 1);
    pixel_3 = get_pixel(image, x + 1, y - 1);
    pixel_4 = get_pixel(image, x - 1, y); // second row
    pixel_5 = get_pixel(image, x + 1, y);
    pixel_6 = get_pixel(image, x -1, y + 1); // 3rd row
    pixel_7 = get_pixel(image, x, y + 1);
    pixel_8 = get_pixel(image, x + 1, y + 1);

    Uint8 a, b, c, d, e, f, g, h;

    SDL_GetRGB(pixel_1, image->format, &a, &a, &a);
    SDL_GetRGB(pixel_2, image->format, &b, &b, &b);
    SDL_GetRGB(pixel_3, image->format, &c, &c, &c);
    SDL_GetRGB(pixel_4, image->format, &d, &d, &d);
    SDL_GetRGB(pixel_5, image->format, &e, &e, &e);
    SDL_GetRGB(pixel_6, image->format, &f, &f, &f);
    SDL_GetRGB(pixel_7, image->format, &g, &g, &g);
    SDL_GetRGB(pixel_8, image->format, &h, &h, &h);

    return (a >= upperthreshold) || (b >= upperthreshold) || (c >= upperthreshold) 
    || (d >= upperthreshold) || (e >= upperthreshold) || (f >= upperthreshold) ||
     (g >= upperthreshold) || (h >= upperthreshold);
}

// Double Thresholding or hysteresis thresholding
void DThresholding(SDL_Surface* image, int x, int y, int upperthreshold, int lowerthreshold)
{
    Uint32 currentpixel, black, white;
    Uint8 rr;

    black = SDL_MapRGB(image->format, 0, 0, 0);
    white = SDL_MapRGB(image->format, 255, 255, 255);

    currentpixel = get_pixel(image, x, y);

    SDL_GetRGB(currentpixel, image->format, &rr, &rr, &rr);

    if (rr < lowerthreshold) // remove weak edge
    {
        put_pixel(image, x, y, black);
    }
    else if (rr >= upperthreshold) // strong edge detected
    {
        put_pixel(image,x,y,white);
    }
    else
    {
        if (true)//Keep(image, x, y, upperthreshold)
        {
            put_pixel(image,x,y,white);
        }
        else
        {
            put_pixel(image, x, y, black);
        }
    }
}

void Canny(char* path, double **M)
{
    SDL_Surface* image = display_bmp(path);
    RemoveLowPixels(image);
    int t = GetAverageGradient(image);
    int upperthreshold = t;
    int lowerthreshold = 1;

    NonMaximumEdgeSuppression(image, M);

    printf("Canny : applying hysteresis thresholding...\n");
    for (int i = 1; i < image->w - 2; i++)
    {
        for (int j = 1; j < image->h - 2; j++)
        {
            DThresholding(image, i, j, upperthreshold, lowerthreshold);
        }
    }

    if (SDL_SaveBMP(image, "Canny.bmp") != 0) // save the grid in .bmp format
    {
        // Error saving Bitmap
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }
    else
    {
        printf("Canny : image saved\n");
    }

    SDL_FreeSurface(image);
}