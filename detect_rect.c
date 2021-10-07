#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdio.h>
#include "detect_rect.h"


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

    char *p = malloc(2);

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

char *Get_Last_Pixel(SDL_Surface *image, int height, int width, int x, int y)
{
    Uint32 pixel = get_pixel(image, x, y);

    Uint8 r, g, b;
    SDL_GetRGB(pixel, image->format, &r, &g, &b);

    while(r < 10 && g < 10 && b < 10 && y < height)
    {
        y++;

        pixel = get_pixel(image, x, y);
        SDL_GetRGB(pixel, image->format, &r, &g, &b);
    }

    y--;
    pixel = get_pixel(image, x, y);
    SDL_GetRGB(pixel, image->format, &r, &g, &b);

    while(r < 10 && g < 10 && b < 10 && x < width)
    {
        x++;

        pixel = get_pixel(image, x, y);
        SDL_GetRGB(pixel, image->format, &r, &g, &b);
    }
    
    x--;
    char *p = malloc(2);

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
        printf("at pixel [%i,j]", i);
        pixel = get_pixel(image, i, j); // Get the value of the pixel at [i, j]
        SDL_GetRGB(pixel, image->format, &r, &g, &b);

        if (r < 10 && g < 10 && b < 10)
        {
            char *p = Get_Highest_Pixel(image, width, height); // Get & compute the rect
            char *q = Get_Last_Pixel(image, width, height, i, j);

            i = q[0]; // update the value of i (optimisation)
        
            int length = p[0] - q[0]; // computation of the length & width of the rect
            int large = p[1] - q[1];

            if (length + large > myrect->h + myrect->w) // if the rect detected is greater than previous
            {
                myrect->h = large; // save the greatest rect
                myrect->w = length;
                myrect->x = p[0];
                myrect->y = p[1];
            }

            if(myrect->h + myrect->w >= (width + height)/2) // opti
            {
                break; // if we can't find a greater square in the image we break + return
            }
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
    SDL_Surface *destination = malloc(sizeof(image)); // destination surface to save

    SDL_Rect *therect = Get_rect(image, image->w, image->h); // get the sudoku rect

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