#ifndef SPLIT_H
#define SPLIT_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include "../image_preprocessing/operations.h"

int square(int x1, int y1, int x2, int y2);

int __detect_rect(SDL_Surface* image,int x1,int y1);

int* detect_rect(SDL_Surface* image);

void invert (SDL_Surface* image);

SDL_Surface* move(SDL_Surface* image, int decalagex, int decalagey);

SDL_Surface* center(SDL_Surface* image);

void __clean(SDL_Surface* image);

void clean(SDL_Surface* image);


void split(SDL_Surface* image, int x0, int y0, int width, int length);

void draw_square(SDL_Surface* image, int x, int y, int w);

#endif
