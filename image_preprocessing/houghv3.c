#include <math.h>
#include "operations.h"
#include "stack.h"
#include <err.h>
#include "houghv3.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

int setPixelVerif(SDL_Surface *Screen,int x, int y)
{
  if (x >= 0 && x < Screen->w &&
      y >= 0 && y < Screen->h)
  {
	  return 0;
  }
  return 1;
}
void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, Uint32 pixel) {
	    int i;
	    double x = x1 - x0;
	    double y = y1 - y0;
	    double length = sqrt( x*x + y*y );
	    double addx = x / length;
	    double addy = y / length;
	    x = x0;
	    y = y0;
	    for ( i = 0; i < length; i += 1) {
		if(setPixelVerif(Screen, x, y) == 0)
		{
			put_pixel(Screen, x, y, pixel );
		}
	        x += addx;
	        y += addy;
	         
	}
}



void HTLineDetection(char* image){
	SDL_Surface* image_surface;
	//SDL_Surface* screen_surface;
	image_surface = display_bmp(image);
	//screen_surface = display_image(image_surface);
	//wait_for_keypressed();
	
	int Nx = image_surface->w;
	int Ny = image_surface->h;
	float rho= 1.0;
	float theta= 1.0;
	int Ntheta= (int) (180.0/theta);
	int Nrho = (int) (floor(sqrt(Nx*Nx + Ny*Ny))/rho);
	float dtheta = 3.14159265 /Ntheta;
	float drho = floor(sqrt(Nx*Nx + Ny*Ny))/Nrho;
	int accum[Ntheta][Nrho];
	for(int x = 0 ; x < Ntheta ; x++)
	{
		for(int y = 0 ; y < Nrho ; y++)
		{
			accum[x][y] = 0;
		}
	}
	for(int y = 0 ; y < Ny ; y++)
	{
		for(int x = 0 ; x < Nx ; x++)
		{
			Uint32 pixel = get_pixel(image_surface, x, y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			if(r != 0){//can change if image not black and white
				for(int i_theta = 0; i_theta < Ntheta; i_theta++)
				{
					theta = i_theta*dtheta;
					rho = (float) (x*cos(theta)) + (float)(Ny - y) * sin(theta);
					int i_rho = (int) (rho/drho);
					if(i_rho > 0)
					{
						if(i_rho < Nrho)
						{
							accum[i_theta][i_rho] += 1;
						}
					}
				}
			}
		}
	}
	SDL_FreeSurface(image_surface);
	image_surface = SDL_CreateRGBSurface(0, Nrho, Ntheta, 32, 0, 0, 0,0);
	//struct line lines[Nrho * Ntheta];
	int len = 0;

	for(int x= 0; x < Nrho; x += 1)
    	{
	    for(int y = 0; y < Ntheta; y += 1)
	    {
		Uint32 pixel = get_pixel(image_surface, x, y);
		int gray = accum[y][x];
		if(gray > 210)
		{	
			pixel = SDL_MapRGB(image_surface->format, gray, gray, gray);
			//struct line Line = {(x * drho), (y * dtheta)};
			//lines[len] = Line;
			len += 1;
		}
		else
		{
			pixel = SDL_MapRGB(image_surface->format, 0, 0, 0);
		}
		put_pixel(image_surface, x, y, pixel);
		}
	}
    /*
	update_surface(screen_surface, image_surface);
    	display_image(image_surface);
    	wait_for_keypressed();
    */
	struct triple* positions = (struct triple*)malloc(sizeof(struct triple ) * 100000);
	positions = analysis(image_surface, 210, &len,positions);
	SDL_FreeSurface(image_surface);
	image_surface = lineTracePos(positions,Nx,Ny,len,dtheta,drho,image);
	//image_surface = lineTrace(image_surface,lines, Nx, Ny, len);
    /*
	update_surface(screen_surface, image_surface);
	display_image(image_surface);
	wait_for_keypressed();	
    */
    SDL_SaveBMP(image_surface,"hough.bmp");
	free(positions);
    	SDL_FreeSurface(image_surface);
    	//SDL_FreeSurface(screen_surface);
}

SDL_Surface* lineTrace(SDL_Surface* surface,struct line lines[], int Nx, int Ny, int len)
{

	surface = SDL_CreateRGBSurface(0, Nx, Ny, 32, 0, 0, 0,0);
	Uint32 pixel = get_pixel(surface,0,0);
	Uint8 r, g, b;
	SDL_GetRGB(pixel, surface->format, &r, &g, &b);
	pixel = SDL_MapRGB(surface->format, 0, 255, 0);
	for(int i = 0 ; i < len ; i++)
	{
		struct line Line = lines[i];
		float ntheta = Line.theta;
		float nrho = Line.rho;

		float a = cos(ntheta);
		float b = sin(ntheta);

		float x0 = a*nrho;
		float y0 = b*nrho;

		int x1 = (int) x0 + 1000*(-b);
		int y1 = (int) y0 + 1000*(a);

		int x2 = (int) x0 - 1000*(-b);
		int y2 = (int) y0 - 1000*(a);
		
		drawLine(surface,x1,y1,x2,y2,pixel);
	}
	return surface;
}

struct triple* analysis(SDL_Surface* image, int seuil,int* len, struct triple *positions)
{
	int Nx = image->w;
	int Ny = image->h;
	int compteur = 0;
	*len = 0;
	SDL_Surface* result = SDL_CreateRGBSurface(0, Nx, Ny, 32, 0, 0, 0,0);
	for(int y = 0 ; y < Ny; y++)
	{
		for(int x = 0; x < Nx ; x++)
		{
	
			Uint32 pixel = get_pixel(image,x,y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, image->format, &r, &g, &b);
			if(r > seuil)
			{
				compteur +=1;
				struct stack* pile = newStack(Nx*Ny);
				struct tupple tupple1 = {x,y};
				push(pile, tupple1);
				float si = 0;

				float sj = 0;
				float npix = 0;
				while(!isEmpty(pile))
				{
					struct tupple tupple2 = pop(pile);
					si += tupple2.e1;
					sj += tupple2.e2;
					npix += 1;
					pile = coloriage_pixel_stack(image, result, Nx,Ny,seuil, pixel, pile,tupple2.e1,tupple2.e2);
				}
				float xb = si*1.0;
				float yb = sj*1.0;
				struct triple position = {xb, yb,npix};
				//printf("%f , %f,%f\n",xb, yb,npix);
				positions[*len] = position;
				*len +=1;
				free(pile->items);
				free(pile);
			}
		}
	}
	//printf("Nombre de tache pour hough : %i\n", compteur);
	SDL_FreeSurface(result);
	return positions;
}

struct stack* coloriage_pixel_stack(SDL_Surface* image, SDL_Surface* result, int Nx, int Ny, int seuil, Uint32 pixel, struct stack* pile, int i, int j)
{

	put_pixel(image, i, j, pixel);
	put_pixel(result, i,j, 255);
	struct tupple couple1 = {i+1, j};
	struct tupple couple2 = {i, j+1};
	struct tupple couple3 = {i-1, j};
	struct tupple couple4 = {i, j-1};
	struct tupple voisins[4] = {couple1,couple2,couple3,couple4};
	for(int b = 0; b < 4 ; b++)
	{
		int k = voisins[b].e1;
		int l = voisins[b].e2;
		if((k>=0) &&(k<Nx) && (l>=0) && (l <Ny))
		{
			Uint32 pixelvoi = get_pixel(image,k,l);
			Uint8 r, g, b;
			SDL_GetRGB(pixelvoi, image->format, &r, &g, &b);
			if(r > seuil)
			{
				put_pixel(image, k, l, pixel);
				push(pile,voisins[b]);
			}
		}
	}
	return pile;
}

SDL_Surface* lineTracePos(struct triple positions[], int Nx, int Ny, int len,float dtheta, float drho, char* path)
{
	SDL_Surface* result = display_bmp(path);
    Nx++;
	Uint32 pixel = get_pixel(result,0,0);
	Uint8 r, g, b;
	SDL_GetRGB(pixel, result->format, &r, &g, &b);
	pixel = SDL_MapRGB(result->format, 255, 0, 0);
	//printf("%i", len);
	for(int i = 0 ; i < len ; i++)
	{
		struct triple pos = positions[i];
		float i_theta = pos.e2;
		float i_rho = pos.e1;
		if(pos.e3<2)
		{

		}
		else 
		{
		float theta = i_theta *dtheta;
		float rho = i_rho*drho;
		float a = cos(theta);
		float b = sin(theta);

		float x0 = a*rho;
		float y0 = b*rho;

		int x1 = (int) x0 + 10000*(b);
		int y1 = (int) y0 + 10000*(-a);

		int x2 = (int) x0 - 10000*(b);
		int y2 = (int) y0 - 10000*(-a);
		
		drawLine(result,x1,Ny-y1,x2,Ny-y2,pixel);
		}

		
	}
	return result;
}
