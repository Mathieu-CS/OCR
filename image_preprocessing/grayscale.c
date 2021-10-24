#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "string.h"
#include "operations.h"
#include "grayscale.h"

void grayscale(char* path)
{
    SDL_Surface* image_surface;

    image_surface = display_bmp(path);

    int width = image_surface->w;
    int height = image_surface->h;


    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            Uint32 pixel = get_pixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            float avg = 0.3*r + 0.59*g + 0.11*b;
            /*if (avg>70) r = g = b = (int)255;
            else
            r = g = b = (int)0;*/
            r = g = b = (int) avg;
            pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface, x, y, pixel);
        }
    }
    SDL_SaveBMP (image_surface , "grayscale.bmp");
    SDL_FreeSurface(image_surface);
}

void black_and_white(char* pathh,char* outputt)
{
    SDL_Surface* image_surface;

    image_surface = display_bmp(pathh);

    int width = image_surface->w;
    int height = image_surface->h;


    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            Uint32 pixel = get_pixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            //float avg = 0.3*r + 0.59*g + 0.11*b;
            if (r+g+b/3>127) r = g = b = (int)255;
            else
            { r = g = b = (int)0;}
           
           
            pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface, x, y, pixel);
        }
        
        
    }
    SDL_SaveBMP (image_surface ,outputt);
   
    SDL_FreeSurface(image_surface);
}