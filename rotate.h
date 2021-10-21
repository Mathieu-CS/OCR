#ifndef ROTATE_H_
#define ROTATE_H_
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include "../prepross/pixel_operations.h"


void Sobel_filter (char *arg,char *arg2);
void manny_filter(char *arg,char *arg2);
void grayscale(char *arg );
void image_rotation(SDL_Surface *image, double angle);



#endif