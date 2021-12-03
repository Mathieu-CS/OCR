#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../image_preprocessing/operations.h"
#include "sdltoarray.h"

void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}


void toArray(char *imageLoc, double imageToArray[])
{
  //printf("%s\n", imageLoc);
  SDL_Surface* image_surface = load_image(imageLoc);
  //int imageToArray[28*28];
  
  int width = image_surface->w;
  int height = image_surface->h;
  for(int y = 0; y < height; y++)
    {
      for(int x = 0; x < width; x++)
	{
	  Uint32 pixel = get_pixel(image_surface, x, y);
	  Uint8 r, g, b;
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

	  if (r != g || g != b)
	    errx(3, "problem with the image");

	  if (r == 255)
	    imageToArray[y*width+x] = 1;
	  else
	    imageToArray[y*width+x] = 0;
	}
    }
  SDL_FreeSurface(image_surface);
  
}

int isDigits2(char *imageLoc)
{
     //printf("%s\n", imageLoc);
  SDL_Surface* image_surface = load_image(imageLoc);
  //int imageToArray[28*28];

  //int max = 784;
  int p = 0;
  
  int width = image_surface->w;
  int height = image_surface->h;
  for(int y = 0; y < height; y++)
    {
      for(int x = 0; x < width; x++)
	{
	  Uint32 pixel = get_pixel(image_surface, x, y);
	  Uint8 r, g, b;
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

	  if (r != g || g != b)
	    errx(3, "problem with the image");

	  if (r > 100)
              p++;
	}
    }
  SDL_FreeSurface(image_surface);

  if (p < 10)
      return 0;
  else
      return 1;
}


void print_matrix(char s[], double m[], size_t rows, size_t cols)
{
  FILE *fp;
  fp = fopen(s, "w+");
  for(unsigned long i = 0; i < rows; i++)
      {
	for(unsigned long j = 0; j < cols; j++)
	  {
	    fprintf(fp, " %2lf", m[i*cols + j]);
	  }
	fprintf(fp, "\n");
      }
  
  fclose(fp);
}
