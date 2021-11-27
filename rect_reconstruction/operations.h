#ifndef SPLIT_H
#define SPLIT_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

int Init_SDL();
Uint32 get_pixel(SDL_Surface *surface, int x, int y);
void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
SDL_Surface* display_bmp(char *file_name);

#endif