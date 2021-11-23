#ifndef HOUGH_TRANSFORM_H
#define HOUGH_TRANSFORM_H

#include "SDL/SDL.h"
#include <math.h>

/*int setPixelVerif(SDL_Surface *Screen,int x, int y);

void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, Uint32 pixel);*/

double Convert(int degree);

void edge_detection(char* path);

#endif