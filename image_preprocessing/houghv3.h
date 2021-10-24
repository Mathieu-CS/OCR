#ifndef HOUGHV3_h
#define HOUGHV3_h

void HTLineDetection(char* image);
struct line{
	float rho;
	float theta;
};
struct triple
{
	float e1;
	float e2;
	float e3;
};

SDL_Surface* lineTrace(SDL_Surface* surface,struct line lines[], int Nx, int Ny, int len);
struct triple* analysis(SDL_Surface* image, int seuil, int *len,struct triple* positions);

struct stack* coloriage_pixel_stack(SDL_Surface* image, SDL_Surface* result, int Nx, int Ny, int seuil, Uint32 pixel, struct stack* pile, int i, int j);
SDL_Surface* lineTracePos(struct triple positions[], int Nx, int Ny, int len,float dtheta, float drho,char* pos);
#endif