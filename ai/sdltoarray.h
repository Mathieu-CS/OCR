#ifndef SDLTOARRAY_H
#define SDLTOARRAY_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

void init_sdl();
SDL_Surface* load_image(char *path);
void toArray(char *imageLoc, double imageToArray[]);
int isDigits2(char *imageLoc);
void print_matrix(char s[], double m[], size_t rows, size_t cols);

#endif
