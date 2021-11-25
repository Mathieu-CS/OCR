#include "SDL/SDL.h"
#include <math.h>
#include "hough_transform.h"
#include "operations.h"
#define pi 3.14159265359

double Convert(int degree)
{
    return degree * (pi / 180);
}

/*int setPixelVerif(SDL_Surface *Screen,int x, int y)
{
    if (x >= 0 && x < Screen->w &&
          y >= 0 && y < Screen->h)
    {
         return 0;
    }
    return 1;
}*/

/*void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, Uint32 pixel)
{
    double x = x1 - x0;
    double y = y1 - y0;
    double length = sqrt( x*x + y*y );
    double addx = x / length;
    double addy = y / length;
    x = x0;
    y = y0;
    for (int i = 0; i < length; i += 1) 
    {
        if (setPixelVerif(Screen, x, y) == 0)
        {
            put_pixel(Screen, x, y, pixel );
        }

        x += addx;
        y += addy;     
    }
}*/

void edge_detection(char* path)
{
    SDL_Surface* image = display_bmp(path);

    int width = image->w;
    int height = image->h;
    int diagonale = floor(sqrt((double) (width * width + height * height)));

    // there will be 180 teta since we go from 0 include to 180 excluded
    // there will be diagonale rho
    int A[diagonale][180];
    for (int i = 0; i < diagonale; i++)
    {
        for (int j = 0; j < 180; j++)
        {
            A[i][j] = 0;
        }
    }
    

    for (double x = 10; x < width-10; x++)
    {
        for (double y = 10; y < height-10; y++)
        {
            Uint32 pixel = get_pixel(image, floor(x), floor(y));
            Uint8 r,g,b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);

            if (r > 200)
            {
                for (int teta = 0; teta < 180; teta++)
                {
                    double tetaRad = Convert(teta);
                    //printf("floor(cos) = %i  teta = %i\n", rho, teta);
                    //if (x * (cos(tetaRad)) - y * (sin(tetaRad) + x * cos(tetaRad) + y * sin(tetaRad)) == 0)
                    {
                        
                        int rho = abs((int) floor(x * cos(tetaRad) + y * sin(tetaRad)));
                        //printf("rho = %i\n", rho);
                        A[rho][teta] += 1;
                        //printf("long thing = %f cos(rho) = %i  cos(tetaRad) = %f  sin(tetaRad) = %f  x = %f  y = %f\n", x * (cos(tetaRad)) - y * (sin(tetaRad)) + rho, rho, cos(tetaRad), sin(tetaRad), x, y);
                        //printf("hello my bwuda 2\n");
                            /*for (int i = -1; i < 2; i++)
                            {
                                for (int j = -1; j < 2; j++)
                                {   
                                    if (rho + i >= 0 && rho + i < diagonale && teta + j >= 0 && teta + j < 180)
                                    {
                                        //printf("hello my bwuda 3\n");
                                        A[rho + i][teta + j] += 1;
                                    }
                                }
                            }*/
                    }
                }
            }
        }
    }
    
    // EDGE DETECTION DONE

    SDL_Surface* houghSpace = SDL_CreateRGBSurface(0, 180, diagonale, 32, 0, 0, 0, 0);

    for (int i = 0; i < diagonale; i++)
    {
        for (int j = 0; j < 180; j++)
        {
            //printf("this : %i\n", A[i][j]);
            if (A[i][j] > 280)
            {
                int value = A[i][j];
                value = (255 * value) / 1;
                //printf("value = %i\n", value);

                Uint32 pixel = SDL_MapRGB(houghSpace->format, (Uint8) value, (Uint8) value, (Uint8) value);
                put_pixel(houghSpace, j, i, pixel);

                int indexk = 0;
                for (int k = -1; k < 2; k++)
                {
                    for (int l = -1; l < 2; l++)
                    {
                        int max = 0;
                        
                        if (i+k >= 0 && i+k < diagonale && j+l >= 0 && j+l < 180 && A[i+k][j+l] > max)
                        {
                            max = A[i+k][j+l];
                            indexk = i+k;
                        }
                    }
                }
                

                    /*for (int x = 0; x < width; x++)
                    {
                        printf("teta = %i\n", j);
                        if (j == 0)
                        {
                            int y = (int) (indexk - x * pi / 2);
                            
                            //printf("x = %i    y = %i\n", x, y);
                            Uint32 pix = SDL_MapRGB(image->format, 178, 34, 34);
                            if (y < 1000 && y >= 0)
                            {
                                put_pixel(image, x, y, pix);
                            }
                            else
                            {
                                //printf("y over 1000 : %i\n", y);
                            }
                        }
                        if (j == 90)
                        {
                            int y = (int) (indexk - x * cos(Convert(j)) / sin(Convert(j)));
                            //printf("x = %i    y = %i\n", x, y);
                            Uint32 pix = SDL_MapRGB(image->format, 178, 34, 34);
                            if (y < 1000 && y >= 0)
                            {
                                put_pixel(image, x, y, pix);
                            }
                            else
                            {
                                //printf("y over 1000 : %i\n", y);
                            }
                        }
                        
                    }*/
                    //if (j >= 80 && j <= 100)
                    {
                        for (int x = 0; x < width; x++)
                        {
                            int y = (int) (indexk - x * cos(Convert(j)) / sin(Convert(j)));
                            //printf("x = %i    y = %i\n", x, y);
                            Uint32 pix = SDL_MapRGB(image->format, 178, 34, 34);
                            if (y < height && y >= 0)
                            {
                                put_pixel(image, x, y, pix);
                            }
                            else
                            {
                                //printf("y over 1000 : %i\n", y);
                            }
                        }
                    }
                    if (j == 0)
                    {
                        for (int y = 0; y < height; y++)
                        {
                            int x = (int) (indexk - y * sin(Convert(j)) / cos(Convert(j)));
                            //printf("x = %i    y = %i\n", x, y);
                            Uint32 pix = SDL_MapRGB(image->format, 178, 34, 34);
                            if (x < width && x >= 0)
                            {
                                put_pixel(image, x, y, pix);
                            }
                            else
                            {
                                //printf("y over 1000 : %i\n", y);
                            }
                        }
                    }
                        
                        
                
                //int x = i * floor(cos(j));
                //int y = i * floor(sin(j));
            }
        }
    }

    SDL_SaveBMP(houghSpace, "houghSpace.bmp");
    SDL_SaveBMP(image, "muchachos.bmp");

    
    
    
}
