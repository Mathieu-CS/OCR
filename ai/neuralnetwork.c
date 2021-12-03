#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <err.h>
#include <string.h>
#include <assert.h>
#include "neuralnetwork.h"
#include "sdltoarray.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#define totalPixels 784

#define numInputs 784
#define numHiddenNodes 15
#define numOutputs 10

#define MAX_LINE_LENGTH 80
#define DATA_SIZE 1000

double relu(double x)
{
  if (x > 0)
    return x;
  else
    return 0.01f*x;
}

double dRelu(double x)
{
  if (x > 0)
    return 1;
  else
    return 0.01f;
}


int neuralNetwork(char imgStr[])
{
  double hiddenLayer[numHiddenNodes];
  double outputLayer[numOutputs];

  double hiddenLayerBias[numHiddenNodes];
  double outputLayerBias[numOutputs];

  double hiddenWeights[numInputs][numHiddenNodes];
  double outputWeights[numHiddenNodes][numOutputs];

  int load = 1;
  
  if (load == 1) // load weights and biases from file
    {
      FILE* fptr;
      fptr = fopen("save.txt", "r");
      
      int j = 0;
      int k = 0;
      double nb;
      while (j < numHiddenNodes)
	{

            int a = fscanf(fptr, "%lf", &nb);
            printf("ici la 2\n");
          if (a != EOF)
              printf("A sup a 0\n");
	  if (k == numInputs)
	    {
	      k = 0;
	      j++;
	    }
	  hiddenWeights[k][j] = nb;
	  k++;
	}
      
      j = 0;
      while (j < numHiddenNodes)
	{
	  hiddenLayerBias[j] = nb;
	  int a = fscanf(fptr, "%lf", &nb);
          if (a != EOF)
              printf("A sup a 0\n");
	  j++;
	}
      
      j = 0;
      k = 0;
      while (j < numOutputs)
	{
	  if (k == numHiddenNodes)
	    {
	      k = 0;
	      j++;
	    }
	  outputWeights[k][j] = nb;
	  if (j != numOutputs-1 || k != numHiddenNodes-1)
	    {
	      int a = fscanf(fptr, "%lf", &nb);
              if (a != EOF)
                  printf("A sup a 0\n");
	    }
	  k++;
	}
      
      j = 0;
      while (j < numOutputs)
	{
	  outputLayerBias[j] = nb;
	  int a = fscanf(fptr, "%lf", &nb);
          if (a != EOF)
              printf("A sup a 0\n");
	  j++;
	}
      
      fclose(fptr);
    }
   
  double imageMatrix[28*28] = {0};
  toArray(imgStr, imageMatrix);
  
  for (int i = 0; i < numHiddenNodes; i++)
    {
      double activation = hiddenLayerBias[i];
      for (int k = 0; k < numInputs ; k++)
	{
	  if (isnan(imageMatrix[k]))
	    errx(1, "probleme ici");
	  activation += imageMatrix[k] * hiddenWeights[k][i];
	}
      hiddenLayer[i] = relu(activation);
    }
  
  double C = outputLayer[0];
  for (int j = 1; j < numOutputs; j++)
    {
      if (outputLayer[j] > C)
	C = outputLayer[j];
    }
  
  double esum = 0.0f;
  for (int j = 0; j < numOutputs; j++)
    {
      double activation = outputLayerBias[j];
      for (int k = 0; k < numHiddenNodes; k++)
	activation += hiddenLayer[k] * outputWeights[k][j];
      double z = activation - C;
      esum += exp(z);
      outputLayer[j] = exp(z);
    }
  
  for (int j = 0; j < numOutputs; j++)
    outputLayer[j] = (outputLayer[j] / (esum));
  
  int maxIndex = 0;
  float maxV = 0;
  for (int i = 0; i < 10; i++)
    {
      if (outputLayer[i] > maxV)
	{
	  maxV = outputLayer[i];
	  maxIndex = i;
	}
    }
  
  return maxIndex;	  
}
