#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdio.h>
#include "detect_rect.h"
#include "operations.h"

/*
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */

Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

/// Get the coordinates of the first pixel of the rect
/// Returns a pointer to an array with [x,y] the coordinates of the pixel

char *Get_Highest_Pixel(SDL_Surface *image, int x, int y)
{
    Uint32 pixel = get_pixel(image, x, y);

    Uint8 r, g, b;
    SDL_GetRGB(pixel, image->format, &r, &g, &b);

    while(r < 10 && g < 10 && b < 10 && y > 0)
    {

        y--;
        
        pixel = get_pixel(image, x, y);
        SDL_GetRGB(pixel, image->format, &r, &g, &b);
    }

    char *p = malloc(2 * sizeof(int));

    if (p == NULL)
    {
        errx(1, "Not enough memory!");
    }

    p[0] = x;
    p[1] = y;

    return p;    
}

/// Get the coordinates of the last pixel of the rect
/// Returns a pointer to an array with [x,y] the coordinates of the pixel

int *Get_Last_Pixel(SDL_Surface *image, int height, int width, int x, int y)
{
    Uint32 pixel = get_pixel(image, x, y);

    Uint8 r, g, b;
    SDL_GetRGB(pixel, image->format, &r, &g, &b);

    while(r < 10 && g < 10 && b < 10 && y < height - 1)
    {
        y++;

        SDL_LockSurface(image);
        pixel = get_pixel(image, x, y);
        SDL_UnlockSurface(image);

        SDL_GetRGB(pixel, image->format, &r, &g, &b);
    }

    pixel = get_pixel(image, x, y);

    SDL_GetRGB(pixel, image->format, &r, &g, &b);

    while(r < 10 && g < 10 && b < 10 && x < width - 1)
    {
        x++;

        pixel = get_pixel(image, x, y);

        SDL_GetRGB(pixel, image->format, &r, &g, &b);
    }

    int *p = malloc(2 * sizeof(int));

    if (p == NULL)
    {
        errx(1, "Not enough memory!");
    }
    
    p[0] = x;
    p[1] = y;
    
    return p;
}

/// Returns the Rect representing the Sudoku

SDL_Rect *Get_rect(SDL_Surface *image, long width, long height)
{
    // Declarations
    int i = 0; // x-axis
    int j = width/2; // y-axis

    Uint32 pixel;
    Uint8 r, g, b;

    SDL_Rect *myrect = malloc(sizeof(*image));

    myrect->h = 0;
    myrect->w = 0;
    myrect->x = 0;
    myrect->y = 0;

    // Loop
    while (i < width)
    {
        //printf("at pixel [%i,%i]\n", i, j);
        
        pixel = get_pixel(image, i, j); // Get the value of the pixel at [i, j]

        SDL_GetRGB(pixel, image->format, &r, &g, &b);

        if (r < 10 && g < 10 && b < 10)
        {
            printf("square detected!\n");

            char *p = Get_Highest_Pixel(image, i, j); // Get & compute the rect
            printf("Highest pixel is at coordinate [%i,%i]\n", p[0], p[1]);
            int *q = Get_Last_Pixel(image, width, height, i, j);
            printf("Lasst pixel is at coordinate [%i,%i]\n", q[0], q[1]);
        
            int length = q[0] - p[0]; // computation of the length & width of the rect
            int large = q[1] - p[1];

            //printf("lenght is (x-axis): %i", length);
            //printf("width is (y-axis): %i", large);

            i = q[0]; // update the value of i (optimisation)

            if (length + large > myrect->h + myrect->w) // if the rect detected is greater than previous
            {
                myrect->h = large; // save the greatest rect
                myrect->w = length;
                myrect->x = p[0];
                myrect->y = p[1];
            }

            if((myrect->h + myrect->w)/2 >= width + height) // opti
            {
                break; // if we can't find a greater square in the image we break + return
            }

            free(q);
            free(p);
        }
        i++; // parameter of loop
    }
    return myrect;
}

/// Final function, crop the image and save the grid to format .bmp

void detect_rect(char *filename)
{
    Init_SDL(); // first things first

    SDL_Surface *image = display_bmp(filename); // load the image

    SDL_Surface *destination;
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

    SDL_LockSurface(image);
    SDL_Rect *therect = Get_rect(image, image->w, image->h); // get the sudoku rect
    SDL_UnlockSurface(image);

    destination = SDL_CreateRGBSurface(0, therect->w, therect->h, 32, rmask, gmask, bmask, amask);
    if (destination == NULL) {
        printf("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
    }
    
    if (SDL_BlitSurface(image, therect, destination, NULL) != 0) // paste the sudoku onto destination surface
    {
        // Error saving Bitmap
        printf("SDL_BlitSurface failed: %s\n", SDL_GetError());
    }

    if (SDL_SaveBMP(destination, "sudoku_grid.bmp") != 0) // save the grid in .bmp format
    {
        // Error saving Bitmap
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }

    //free the memory
    SDL_FreeSurface(image);
    SDL_FreeSurface(destination);
    free(therect);
}