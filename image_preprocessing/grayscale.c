#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "string.h"
#include "operations.h"
#include "grayscale.h"

void grayscale(char* path)
{
    SDL_Surface* image_surface;

    image_surface = display_bmp(path);

    int width = image_surface->w;
    int height = image_surface->h;


    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            Uint32 pixel = get_pixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            float avg = 0.3*r + 0.59*g + 0.11*b;
            /*if (avg>70) r = g = b = (int)255;
            else
            r = g = b = (int)0;*/
            r = g = b = (int) avg;
            pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface, x, y, pixel);
        }
    }
    SDL_SaveBMP (image_surface , "grayscale.bmp");
    SDL_FreeSurface(image_surface);
}
int max(int m1, int m2)
{
  return (m1 > m2) ? m1 : m2;
}

int min(int m1, int m2)
{
  return (m1 < m2) ? m1 : m2;
}

void free_array(Uint32 **array, int width)
{
  int i;
  for(i = 0; i < width; i++)
    {
      free(array[i]);
    }
  free(array);
}

void Threshold_adaptative(char *input)

{
    SDL_Surface* image;

    image = display_bmp(input);
  SDL_Surface *to_return = NULL;
  to_return = image;

  int width = image->w;
  int height = image->h;

  Uint32 sum = 0;

  Uint32 **int_img;

  int_img = malloc(sizeof(Uint32*)*width);

  for(int i = 0; i < width; i++)
      int_img[i] = malloc(sizeof(Uint32)*height);

  for(int i = 0; i < width; i++)
    {
      sum = 0;
      for(int j = 0; j < height; j++)
    {
      Uint32 pixel = get_pixel(image, i, j);
      Uint8 r, g, b;
      SDL_GetRGB(pixel, image->format, &r, &g, &b);
      sum = sum + r;
      

      if(i == 0)
        {
          int_img[i][j] = sum;
        }
      else
        { 
          int_img[i][j] = sum + int_img[i - 1][j];
        }
    }
    }

  int s = width / 8;
  int s2 = s / 2;
  int t = 15;
  Uint32 count = 0;

  for(int i = 0; i < width; i++)
    {
      for(int j = 0; j < height; j++)
    {
      int y0 = max(j - s2, 0);
      int y1 = min(j + s2, height - 1);
      int x0 = max(i - s2, 0);
      int x1 = min(i + s2, width - 1);
      
      count = (y1 - y0) * (x1 - x0);
      
      sum = int_img[x1][y1] - int_img[x1][y0] - int_img[x0][y1] + int_img[x0][y0];

      //printf("here\n");
      Uint32 pixelT = get_pixel(image, i, j);
      Uint8 r, g, b;

      SDL_GetRGB(pixelT, image -> format, &r, &g, &b);

      if((r * count) <= (sum * (100 - t) / 100))
        {
          Uint32 pixel = SDL_MapRGB(image->format, 255, 255, 255);
          put_pixel(to_return, i, j, pixel);
        }

      else
        {
          Uint32 pixel = SDL_MapRGB(image->format, 0, 0, 0);
          put_pixel(to_return, i, j, pixel);
        }
    }
    }

  SDL_SaveBMP(to_return, "blackwhite.bmp");
  SDL_FreeSurface(to_return);
  //SDL_FreeSurface(image);

  free_array(int_img, width);
}
void TreshHolddd (char *input)
{
    SDL_Surface* image;

    image = display_bmp(input);

    int width = image->w;
    int height = image->h;

    for (int x = 1; x < width-1; x++)
    {
        for (int y = 1; y < height-1; y++)
        {
            Uint32 pixel_1 = get_pixel(image, x - 1, y - 1); 
            Uint32 pixel_2 = get_pixel(image, x - 1, y);
            Uint32 pixel_3 = get_pixel(image, x - 1, y + 1);
            Uint32 pixel_4 = get_pixel(image, x , y - 1);
            Uint32 pixel_5 = get_pixel(image, x , y);
            Uint32 pixel_6 = get_pixel(image, x , y + 1);
            Uint32 pixel_7 = get_pixel(image, x + 1, y - 1);
            Uint32 pixel_8 = get_pixel(image, x + 1, y);
            Uint32 pixel_9 = get_pixel(image, x + 1, y + 1);

            /*
            [1][4][7]                           
            [2][5][8]  the pixel references    
            [3][6][9]                           
            */
            Uint8 a, b, c, d, e, f, g ,h, i;

           

            SDL_GetRGB(pixel_1, image->format, &a, &a, &a);
            SDL_GetRGB(pixel_2, image->format, &b, &b, &b);
            SDL_GetRGB(pixel_3, image->format, &c, &c, &c);
            SDL_GetRGB(pixel_4, image->format, &d, &d, &d);
            SDL_GetRGB(pixel_5, image->format, &e, &e, &e);
            SDL_GetRGB(pixel_6, image->format, &f, &f, &f);
            SDL_GetRGB(pixel_7, image->format, &g, &g, &g);
            SDL_GetRGB(pixel_8, image->format, &h, &h, &h);
            SDL_GetRGB(pixel_9, image->format, &i, &i, &i);
            double surrounding_average= (a + b + c + d + e+ f + g + h + i) /9;

            if (e >= surrounding_average)
            {
                pixel_5 = SDL_MapRGB(image->format, 255, 255, 255);
                put_pixel(image, x, y, pixel_5);
            }
            else
            {   pixel_5 = SDL_MapRGB(image->format, 0, 0, 0);
                put_pixel(image, x, y, pixel_5);
            }
        }
    }
    SDL_SaveBMP (image ,"blackwhite.bmp");
   
    SDL_FreeSurface(image);
    
}

void black_and_white(char* pathh,char* outputt)
{
    SDL_Surface* image_surface;

    image_surface = display_bmp(pathh);

    int width = image_surface->w;
    int height = image_surface->h;


    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            Uint32 pixel = get_pixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            //float avg = 0.3*r + 0.59*g + 0.11*b;
            if (r+g+b/3>127) r = g = b = (int)255;
            else
            { r = g = b = (int)0;}
           
           
            pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface, x, y, pixel);
        }
        
        
    }
    SDL_SaveBMP (image_surface ,outputt);
   
    SDL_FreeSurface(image_surface);
}