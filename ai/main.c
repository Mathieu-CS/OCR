#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "sdltoarray.h"
#include "neuralnetwork.h"


int main()
{
  init_sdl();

  //if (isDigits("img/11.bmp"))
  printf("IMAGE : %d\n", neuralNetwork("../0.bmp"));
  return 0;
}
