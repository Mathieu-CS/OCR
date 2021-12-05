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

#define LR 0.01f
#define T 1
#define totalPixels 784

#define numInputs 784
#define numHiddenNodes 15
#define numOutputs 10

#define MAX_LINE_LENGTH 80

#define DATA_SIZE 1000
#define NBIMG 1210

#define EPOCHS 2

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

// Activation function
double sigmoid(double x)
{
  return 1 / (1 + exp(-x));
}

// Derivative
double dSigmoid(double x)
{
  return x * (1 - x);
}

// Init all weights and biases between 0.0 and 1.0
double init_weight(int n)
{
  if (n == 0)
    return ( (double) rand() ) / ( (double) RAND_MAX) * sqrt((double) ((double) 2.0f/ (double) numInputs));
  else if (n == 1)
    return ( (double) rand() ) / ( (double) RAND_MAX) * sqrt((double) ((double) 2.0f/ (double) numHiddenNodes));
  else
    return 0.0f;
}

void shuffle(char inputs[NBIMG][27],  double outputs[NBIMG], size_t n)
{
  if (n > 1)
    {
      size_t i;
      for (i = 0; i < n - 1; i++)
	{
	  //srand( time(NULL) );
	  size_t j = i + rand() / ( RAND_MAX / ((n - 1 - i) + 1));
	  int t = outputs[j];
	  outputs[j] = outputs[i];
	  outputs[i] = t;
	  
	  char g[27];
	  char h[27];

	  strcpy(g, inputs[i]);
	  strcpy(h, inputs[j]);
	  strcpy(inputs[j], g);
	  strcpy(inputs[i], h);


	}
    }
}


void save(char filename[], double hiddenWeights[numInputs][numHiddenNodes], double hiddenLayerBias[numHiddenNodes],
	  double outputWeights[numHiddenNodes][numOutputs], double outputLayerBias[numOutputs])
{
  FILE* fPtr;

  fPtr = fopen(filename, "w");

  if (fPtr == NULL)
    errx(1, "Unable to create file.\n");
    
  //Final Hidden Weights
  for (int j=0; j<numHiddenNodes; j++) {
    for(int k=0; k<numInputs; k++) {
      fprintf(fPtr, "%f ", hiddenWeights[k][j]);
    }
  }
  fprintf(fPtr, "\n");
    
  //Hidden Biases
  for (int j=0; j<numHiddenNodes; j++) {
    fprintf(fPtr, "%f ", hiddenLayerBias[j]);
	
  }
  
  fprintf(fPtr, "\n");
  
  // Output Weights
  for (int j=0; j<numOutputs; j++) {
    for (int k=0; k<numHiddenNodes; k++) {
      fprintf(fPtr, "%f ", outputWeights[k][j]);
    }
  }
  
  fprintf(fPtr, "\n");

  // Output Layer Biases
  for (int j=0; j<numOutputs; j++) {
    fprintf(fPtr, "%f ", outputLayerBias[j]);
        
  }
  
  fprintf(fPtr, "\n");

  fclose(fPtr);
  
}


void loadTraining(char path[], double arr[])
{
  char line[MAX_LINE_LENGTH] = {0};
  FILE *file = fopen(path, "r");
  int i = 0;
  
  while(fgets(line, MAX_LINE_LENGTH, file))
    {
      arr[i] = atoi(line);
      i++;
    }
  fclose(file);
}

void neuralNetwork(int SL)
{
  int trainingnb = NBIMG;

  unsigned long error = 0;
  unsigned long total = EPOCHS * trainingnb;
  
  double hiddenLayer[numHiddenNodes];
  double outputLayer[numOutputs];

  double hiddenLayerBias[numHiddenNodes];
  double outputLayerBias[numOutputs];

  double hiddenWeights[numInputs][numHiddenNodes];
  double outputWeights[numHiddenNodes][numOutputs];

  FILE* fptr2 = fopen("save.txt", "r");
  int fileExist = 0;
  if(fptr2 != NULL)
    {
      fileExist = 1;
      fclose(fptr2);
    }
  
  
  if ((SL == 2 || SL == 3) && fileExist == 1)
    {
      FILE* fptr;
      fptr = fopen("save.txt", "r");
      
      int j = 0;
      int k = 0;
      double nb;
      while (j < numHiddenNodes)
	{
	  int a = fscanf(fptr, "%lf", &nb);
	  if (a < 0)
	    printf("Problem\n");
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
	  if (a < 0)
	    printf("Problem\n");
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
	      if (a < 0)
		printf("Problem\n");
	    }
	  k++;
	}
      
      j = 0;
      while (j < numOutputs)
	{
	  outputLayerBias[j] = nb;
	  int a = fscanf(fptr, "%lf", &nb);
	  if (a == -99)
	    printf("Problem\n");
	  j++;
	}
      
      fclose(fptr);
    }
  else
    {
      for (int i = 0; i < numInputs; i++)
	{
	  for (int j = 0; j < numHiddenNodes; j++)
	    {
	      hiddenWeights[i][j] = init_weight(0);
	    }
	}
      
      for (int i = 0; i < numHiddenNodes; i++)
	{
	  for (int j = 0; j < numOutputs; j++)
	    {
	      outputWeights[i][j] = init_weight(1);
	    }
	}
      for (int i = 0; i < numHiddenNodes; i++)
	{
	  hiddenLayerBias[i] = init_weight(0);
	}
      
      for (int i = 0; i < numOutputs; i++)
	{
	  outputLayerBias[i] = init_weight(1);
	}
    }
      
  char training_inputs[NBIMG][27] = {0};
  for(int i = 0; i < NBIMG; i++)
    {
      char temp[27];
      sprintf(temp, "training_db/%05d.bmp", i+1);
      strcpy(training_inputs[i], temp);
    }

  double training_outputs[NBIMG] = {0};
  loadTraining("training_db.txt", training_outputs);
  
  for (int n = 0; n < EPOCHS; n++)
    {
      shuffle(training_inputs, training_outputs, trainingnb);

      if (n % 5 == 0)
	{
	  if (SL == 1 || SL == 3)
	    save("save.txt", hiddenWeights, hiddenLayerBias, outputWeights, outputLayerBias);
	}

      for (int x = 0; x < trainingnb; x++)
	{
	  double imageMatrix[28*28] = {0};
	  toArray(training_inputs[x], imageMatrix);
	  
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

	  // softmax
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
		{
		  activation += hiddenLayer[k] * outputWeights[k][j];
		}
	      double z = activation - C;

	      esum += exp(z);
	      outputLayer[j] = exp(z);
	    }

	  for (int j = 0; j < numOutputs; j++)
	    {
	      outputLayer[j] = (outputLayer[j] / (esum));
	    }
	  


	  int maxIndex = 0;
	  float maxV = 0;
	  //printf("%d", n);
	  for (int i = 0; i < 10; i++)
	    {
	      //printf(" %f ", outputLayer[i]);
	      if (outputLayer[i] > maxV)
		{
		  maxV = outputLayer[i];
		  maxIndex = i;
		}
	    }
	  // printf("\n");



          if (maxIndex != training_outputs[x])
              error += 1;
          
	  printf("Input: %s    Output: %d   Expected Output: %f \n \n", training_inputs[x], maxIndex, training_outputs[x]);
      
	  // Backprop
	  double DeltaOutput[numOutputs];

	  for (int j = 0; j < numOutputs; j++)
	    {    
	      double dError;
	      if (j == training_outputs[x])
	      	dError = (outputLayer[j]-1);
	      else
	      	dError = outputLayer[j];

	      DeltaOutput[j] = dError * dRelu(outputLayer[j]);

	    }

	  double deltaHidden[numHiddenNodes];
	  for (int j = 0; j < numHiddenNodes; j++)
	    {
	      double dError = 0.0f;
	      for (int k = 0; k < numOutputs; k++)
		{
		  dError += DeltaOutput[k] * outputWeights[j][k];
		}
	      deltaHidden[j] = dError * dRelu(hiddenLayer[j]);
	    }

	  for (int j = 0; j < numOutputs; j++)
	    {
	      outputLayerBias[j] -= DeltaOutput[j]*LR;

	      if (isnan(outputLayerBias[j]))
		    {
		      save("probleme.txt", hiddenWeights, hiddenLayerBias,
			   outputWeights, outputLayerBias);
		      errx(1, "ya un prob output layer bias, delta output : % lf\n", DeltaOutput[j]);
		    }
	      
	      for (int k = 0; k < numHiddenNodes; k++)
		{
		  outputWeights[k][j] -= hiddenLayer[k] * DeltaOutput[j] * LR;

		  if (isnan(outputWeights[k][j]))
		    {
		      save("probleme.txt", hiddenWeights, hiddenLayerBias,
			   outputWeights, outputLayerBias);
		      errx(1, "ya un prob output weights\n");
		    }
		}
	    }

	  // Apply change in hidden weights
	  for (int j = 0; j < numHiddenNodes; j++)
	    {
	      hiddenLayerBias[j] -= deltaHidden[j]*LR;
	      if (isnan(hiddenLayerBias[j]))
		{
		  save("probleme.txt", hiddenWeights, hiddenLayerBias,
		       outputWeights, outputLayerBias);
		  errx(1, "ya un prob hidden layer bias\n");
		}
	      for (int k = 0; k < numInputs; k++)
		{  
		  hiddenWeights[k][j] -= imageMatrix[k] * deltaHidden[j] * LR;
		  
		  if (isnan(hiddenWeights[k][j]))
		    {
		      save("probleme.txt", hiddenWeights, hiddenLayerBias,
			   outputWeights, outputLayerBias);
		      errx(1, "ya un prob hidden weights\n");
		    }
		}
	    }
	  
	}
    }


  printf("\nPERCENT (CORRECT) : %lf%% \n", (double) (((double)total-error)*100)/total);

   if (SL == 1 || SL == 3)
    save("save.txt", hiddenWeights, hiddenLayerBias, outputWeights, outputLayerBias);

}
