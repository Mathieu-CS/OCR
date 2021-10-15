#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "string.h"
#include "pixel_operations.h"
#include "grayscale.h"


/*void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}
*/

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
/*

SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}
*/
/*
void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}*/


void grayscale(char* path,char* output)
{
    SDL_Surface* image_surface;
    //SDL_Surface* screen_surface;

    // TODO: Initialize the SDL
    //init_sdl();

    image_surface = load_image(path);
    // TODO: Display the image.
    //screen_surface = display_image(image_surface);

    // TODO: Wait for a key to be pressed.
    //wait_for_keypressed();

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
    SDL_SaveBMP (image_surface ,output);
   // update_surface(screen_surface, image_surface);
    // TODO: Wait for a key to be pressed.
    //wait_for_keypressed();

    // TODO: Free the image surface.
    SDL_FreeSurface(image_surface);

    // TODO: Free the screen surface.
    //SDL_FreeSurface(screen_surface);
    

    
}
void black_and_white(char* pathh,char* outputt)
{
    SDL_Surface* image_surface;
    //SDL_Surface* screen_surface;

    // TODO: Initialize the SDL
    //init_sdl();

    image_surface = load_image(pathh);
    // TODO: Display the image.
    //screen_surface = display_image(image_surface);

    // TODO: Wait for a key to be pressed.
    //wait_for_keypressed();

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
int * sort_1dim_array(int* arr)
{
    int  temp, i, j;
    int n=strlen(arr);
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (arr[i] > arr[j]) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}
(int*,(int,int)*) image_to_array(char * chemin)   //takes a grayscale image
{
   
    SDL_Surface* image_surface;
    image_surface = load_image(chemin);
    int width = image_surface->w;
    int height = image_surface->h;
    int * arr[height*width];
    (int,int)* array[height*width];
    int i=0;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            Uint32 pixel = get_pixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            arr[i]=r;
            array[i]=(x,y);
            i++;  
        }
        
        
    }
    return (arr,array);


}
