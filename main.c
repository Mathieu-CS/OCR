#include <err.h>
#include <stdio.h>
#include <math.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "operations.h"
#include "grayscale.h"
#include "rotate.h"
#include "noise_cancel.h"
#include "Gauss.h"
#include "Canny.h"
#include "Sobel.h"

int main()
{
    //grayscale
    grayscale("Images/image_06.jpeg");

    // noise removing 
    Gamma("grayscale.bmp");
    Contrast("gamma.bmp");
    Gauss("contrast.bmp");

    SDL_Surface* image = display_bmp("Gauss.bmp");

    double **M = calloc(image->w, sizeof(double)); // initialisation of the Matrix

    for (int i = 0; i < image->w; i++)
    {
        M[i] = calloc(image->h, sizeof(double));
    } // end of init

    // line detection
    Sobel("Gauss.bmp", M);
    Canny("Sobel.bmp", M);

    //TODO Hough

    for (int k = 0; k < image->w; k++) // free the Matrix
    {
        free(M[k]);
    }

    return 0;
}