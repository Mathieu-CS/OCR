
#include "pixel_operations.h"
#include <math.h>
#include <stdlib.h>
#include "noise_cancel.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

int image_pixel_average(SDL_Surface *image)
{
    int sum_red = 0;
    int sum_green = 0;
    int sum_blue = 0;

    for (int x = 0; x < image->w; x++)
    {
        for (int y = 0; y < image->h; y++)
        {
            uint32_t pixel =  get_pixel(image, x , y);
            uint8_t r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            sum_blue += b;
            sum_green += g;
            sum_red += r;
        }
    }

    int numberPixels = image->h * image->w;
    SDL_UnlockSurface(image);
    return (sum_red / numberPixels + sum_green / numberPixels + sum_blue / numberPixels) / 3;
}

int Truncate(int value)
{
    if (value < 0)
        return 0;

    if (value > 255)
        return 255;
    
    return value;
}

uint8_t Min(uint8_t r, uint8_t g, uint8_t b)
{
    if (r < g)
        if (r < b)
            return r;
    if (g < b)
        return g;
    return b;    
}

void Contrast(SDL_Surface *image)
{
    int c_value = image_pixel_average(image);
    int av = image_pixel_average(image);

    for (int x = 0; x < image->w; x++)
    {
        for (int y = 0; y < image->h; y++)
        {
            Uint8 r, g, b;
            Uint32 pixel = get_pixel(image, x, y);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);

            double factor = (259 * (c_value + 255)) / (255.0* (259.0 - c_value));

            int new_red = Truncate(factor * (r -av) + av);
            int new_green = Truncate(factor * (g - av) + av);
            int new_blue = Truncate(factor * (b - av) + av);
            int mini = Min(new_red, new_green, new_blue);

            pixel = SDL_MapRGB(image->format, mini, mini, mini);
            put_pixel(image, x, y, pixel);
        }
    }
    SDL_UnlockSurface(image);
    SDL_SaveBMP(image, "contrast.bmp");
    SDL_FreeSurface(image);
}



void Gamma(SDL_Surface *image)
{
    float gamma_correction = 2.0f / (float) (255 - image_pixel_average(image));

    for (int x = 0; x < image->w; x++)
    {
        for (int y = 0; y < image->h; y++)
        {
            uint32_t pixel = get_pixel(image, x, y);
            uint8_t r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);

            int new_red = Truncate(pow((float) (r / 255.0), gamma_correction) * 255.0f);
            int new_green = Truncate(pow((float) (g / 255.0), gamma_correction) * 255.0f);
            int new_blue = Truncate(pow((float) (b / 255.0), gamma_correction) * 255.0f);

            pixel = SDL_MapRGB(image->format, new_red, new_green, new_blue);
            put_pixel(image, x, y, pixel);
        }
    }
    
    SDL_UnlockSurface(image);
    SDL_SaveBMP(image, "gamma.bmp");
    SDL_FreeSurface(image);
}


