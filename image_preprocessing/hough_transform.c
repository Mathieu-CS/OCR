#include "SDL/SDL.h"
#include <math.h>
#include "hough_transform.h"
#include "operations.h"
#include "rotate.h"
#include <SDL/SDL_rotozoom.h>
#define pi 3.14159265359

double Convert(int degree)
{
    return degree * (pi / 180);
}

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
                    {      
                        int rho = abs((int) floor(x * cos(tetaRad) + y * sin(tetaRad)));

                        A[rho][teta] += 1;
                    }
                }
            }
        }
    }
    
    // EDGE DETECTION DONE
    //
	int *tetas = calloc(180, sizeof(int));
	for (int i = 0; i < 180; i++)
	{
		for(int j = 0; j < diagonale; j++)
		{
			if (A[j][i] > 250)
			{
				tetas[i]++;
			}
		}
	}

	int maxVertical = 45;
	int maxHorizontal = 0;

	for (int i = 0; i < 180; i++)
	{
		if (i > 45 && i < 135)
		{
			if (tetas[maxVertical] < tetas[i])
				maxVertical = i;
		}
		else
		{
			if (tetas[maxHorizontal] < tetas[i])
				maxHorizontal = i;
		}
	}
	printf("maxVertical = %i\n", maxVertical);
	printf("maxHorizontal = %i\n", maxHorizontal);

        SDL_Surface* blackwhite = display_bmp("blackwhite.bmp");
        blackwhite = rotozoomSurface(blackwhite, maxHorizontal, 1.0, 2);
        SDL_SaveBMP(blackwhite, "blackwhite.bmp");
        SDL_FreeSurface(blackwhite);

    image = rotozoomSurface(image, maxHorizontal, 1.0, 2);

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
                    {
                        
                        int rho = abs((int) floor(x * cos(tetaRad) + y * sin(tetaRad)));
                        A[rho][teta] += 1;
                    }
                }
            }
        }
    }

    SDL_Surface* houghSpace = SDL_CreateRGBSurface(0, 180, diagonale, 32, 0, 0, 0, 0);
    int threshold;

    for (int i = 0; i < diagonale - 9; i+=10)
    {
        for (int j = 0; j < 180; j++)
        {
            int indexk = 0;
            int max = 0;
            for (int k = 0; k < 10; k++)
                {
                    {
                        
                        if (i+k >= 0 && i+k < diagonale && A[i+k][j] > max)
                        {
                            
                            max = A[i+k][j];
                            indexk = i+k;
                            
                        }
                    }
                }
            threshold = 200;
            if (abs(maxHorizontal) > 4)
            {
                threshold = 60;
            }
            
            if (A[indexk][j] > threshold)
            {
                int value = A[i][j];
                value = (255 * value) / 1;

                Uint32 pixel = SDL_MapRGB(houghSpace->format, (Uint8) value, (Uint8) value, (Uint8) value);
                put_pixel(houghSpace, j, i, pixel);

                    if (j == 90)
                    {
                        for (int x = 0; x < width; x++)
                        {
                            int y = (int) (indexk - x * cos(Convert(j)) / sin(Convert(j)));
                            Uint32 pix = SDL_MapRGB(image->format, 178, 34, 34);
                            if (y < height && y >= 0)
                            {
                                Uint32 inter_pixel = get_pixel(image, floor(x), floor(y));
                                Uint8 r, g, b;
                                SDL_GetRGB(inter_pixel, image->format, &r, &g, &b);
                                if (r != 178)
                                    put_pixel(image, x, y, pix);
                                else
                                    put_pixel(image, x, y, SDL_MapRGB(image->format, 0, 255, 0));
                            }
                        }
                    }
                    if (j == 0)
                    {
                        for (int y = 0; y < height; y++)
                        {
                            int x = (int) (indexk - y * sin(Convert(j)) / cos(Convert(j)));
                            Uint32 pix = SDL_MapRGB(image->format, 178, 34, 34);
                            if (x < width && x >= 0)
                            {
                                Uint32 inter_pixel = get_pixel(image, floor(x), floor(y));
                                Uint8 r, g, b;
                                SDL_GetRGB(inter_pixel, image->format, &r, &g, &b);
                                if (r != 178)
                                    put_pixel(image, x, y, pix);
                                else
                                    put_pixel(image, x, y, SDL_MapRGB(image->format, 0, 255, 0));
                            }
                        }
                    }
            }
        }
    }
;
    SDL_SaveBMP(houghSpace, "houghSpace.bmp");
    SDL_SaveBMP(image, "muchachos.bmp");

    SDL_FreeSurface(houghSpace);
    SDL_FreeSurface(image);
    
    
}
