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
#include "houghv3.h"
#include "stack.h"

int main(int argc,char **argv)//argv[1]=path to image; argv[2]=degree; argv[3]= 1 if we use gamma/ something else if we don't
{
    
    argc++;

    if (argv[2][0]=='r')
     {
        image_rotation(argv[1],35);
     }
     else{image_rotation(argv[1],0);}

    
    //grayscale and gauss
    
    grayscale("rotated.bmp");
    Gauss("grayscale.bmp");
    SDL_Surface* image = display_bmp("Gauss.bmp");

    double **M = calloc(image->w, sizeof(double)); // initialisation of the Matrix

    for (int i = 0; i < image->w; i++)
    {
        M[i] = calloc(image->h, sizeof(double));
    } // end of init
    //noise removing 
    if (argv[3][0]=='g')
    {
        Gamma("Gauss.bmp");
        Contrast("gamma.bmp");
        Threshold_adaptative("contrast.bmp");
        Sobel("blackwhite.bmp", M);
        HTLineDetection("Sobel.bmp");
        
    }
    else //argv[3]!="gamma"
    {
        Threshold_adaptative("Gauss.bmp");
        Sobel("blackwhite.bmp", M);
        HTLineDetection("Sobel.bmp");
        
    }
     
    // line detection
    
    Canny("Sobel.bmp", M);
    

    

    for (int k = 0; k < image->w; k++) // free the Matrix
    {
        free(M[k]);
    }

    return 0;
}