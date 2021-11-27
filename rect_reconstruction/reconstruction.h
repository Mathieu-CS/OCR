#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "reconstruction.h"
#include "SDL/SDL_rotozoom.h"

void reconstruction(char* infos, char* solved, SDL_Surface* baseimage);

#endif