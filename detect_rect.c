// This file holds the functions that will be used in split.c

// includes for SDL

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

// mandatory function to use SDL

void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

// Loads an image in the SDL_Surface format.

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

// Returns the pixel at coordinates (x,y)

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}

// returns an array with [x, y] being the coordinates of the bottom right pixel of the rect

char *GetLastPixelCoor(SDL_surface *img, width, height, i, j)
{
    int x = i;
    int y = j;

    char *ret = malloc(w);
    
    if(!ret)
    {
        return NULL;
    }

    Uint32 pixel = get_pixel(img, x, y);

    Uint8 r, g, b;
    SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

    while(r == 0 && g == 0 && b == 0) // détection du côté haut
    {
        ret[0] = x;
        x++;

        if(x < width) // safe check
        {
            Uint32 pixel = get_pixel(img, x, y);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
        }
        else
        {
            break;
        }
    }

    x = ret[0];

    while(r == 0 && g == 0 && b == 0) // détection du côté droit
    {
        ret[1] = y;
        y++;

        if(y < height) // safe check
        {
            Uint32 pixel = get_pixel(img, x, y);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
        }
        else
        {
            break;
        }
    }

    ret[0] = ret[0] - i;
    ret[1] = ret[1] - j;

    return ret;
}

// Returns the Rect corresponding to the sudoku grid in the surface *img;

SDL_Rect get_grid(SDL_surface *img)
{
    long width = img->w;
    long height = img->h;

    SDL_Rect myrect; // the final rect we will return

    myrect.h = 0;   // initialize the values of myrect
    myrect.w = 0;
    myrect.x = 0;
    myrect.y = 0;

    for(long i = 0; i < width; i++)
    {
        for(long j = 0; j < height; j++)
        {
            // Get the Pixel value at [i,j]
            Uint32 pixel = get_pixel(img, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            if(r == 0 && g == 0 && b == 0) // if the rgb value of the pixel is (0,0,0) (black)
            {
                int arr[] = GetLastPixelCoor(img, width, height, i, j);

                SDL_Rect temprect;

                temprect.x = i;
                temprect.y = j;
                temprect.h = arr[1];
                temprect.w = arr[0];

                if(temprect.h + temprect.w > myrect.h + myrect.w) // if the Rect as a greater perimeter than myrect
                {
                    myrect = temprect; // Store the value of Rect in myrect
                    i = temprect.w + temprect.x; // set i at i + Rect->w and j at j + Rect->h
                    j = temprect.h + temprect.y;
                }
            }
        }
    }

    return myrect;
}

void split_and_save() // crops the sudoku grid from the original image and save it.
{
    // Load the image

    // call get_grid and save the rect result

    // use the rect result with SDL_BlitSurface() to crop the grid and save it onto a new surface

    // Save the surface with SDL_SaveBMP()
}

void main(char *path) // the only purpose of this function is to test the work done here
{

}