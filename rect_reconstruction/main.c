#include "reconstruction.h"
#include "operations.h"

int main()
{
    SDL_Surface* basesurface = display_bmp("../Images/image_01.jpeg");
    reconstruction("infos", "grid_01.result", basesurface);

}