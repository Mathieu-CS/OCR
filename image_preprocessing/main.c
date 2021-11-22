#include <err.h>
#include <stdio.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "operations.h"
#include "grayscale.h"
#include "rotate.h"
#include "noise_cancel.h"
#include "Gauss.h"
#include "Canny.h"
#include "Sobel.h"
#include "houghv3.h"
#include "stack.h"
#include "houghv4.h"
#include "image.h"
#include "int_list.h"

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
    //Gauss("grayscale.bmp");
    
    
    if (argv[3][0]=='g')
    {
        
        Gamma(argv[1]);
        Contrast("gamma.bmp");
        Gauss("contrast.bmp");
        SDL_Surface* image = display_bmp("Gauss.bmp");

        double **M = calloc(image->w, sizeof(double)); // initialisation of the Matrix

        for (int i = 0; i < image->w; i++)
        {
        M[i] = calloc(image->h, sizeof(double));
        } // end of init
        //noise removing 
        otsu_treshold("gauss.bmp");
        Sobel("blackwhite.bmp", M);
        //augusiin hough

    	
        int_list listx= *li_create();
        int_list listy= *li_create();
        SDL_Surface* myimagesurface = display_bmp("Sobel.bmp");
        Image myimage_image = SDL_Surface_to_Image(myimage_image);
        char* verbose_path ="../Images/"
        **hough_transform(myimage_image,  myimage_image, listx, listy, true, verbose_path)
        for (int k = 0; k < image->w; k++) // free the Matrix
        {
        free(M[k]);
        }

        
    }
    else //argv[3]!="gamma"
    {
        otsu_treshold("Gauss.bmp");
       // Sobel("blackwhite.bmp", M);
        HTLineDetection("Sobel.bmp");
        
    }
     
    // line detection
    
    //Canny("Sobel.bmp", M);
    

    

    
    return 0;
}
