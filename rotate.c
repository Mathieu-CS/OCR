#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include "../prepross/pixel_operations.h"


void image_rotation(SDL_Surface *image, double angle)
{
  SDL_Surface *rotation = NULL;

  rotation = rotozoomSurface(image, angle, 1.0, 2);

  SDL_SaveBMP(rotation, "rotated.bmp");
  SDL_FreeSurface(image);
  SDL_FreeSurface(rotation);
}
int main ()
{
    SDL_Surface* image_surface;
    image_surface = load_image("../prepross/Images/image_05.jpeg");
    image_rotation(image_surface,35);//35 is the right degree to rotate image 5 
}
