#ifndef CANNY_H_
#define CANNY_H_

#include <math.h>

void NonMaximumEdgeSuppression(SDL_Surface* image, double** M);
void Canny(char* path, double **M);

#endif