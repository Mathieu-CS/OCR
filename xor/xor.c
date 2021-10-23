#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LR 0.1f
#define EPOCHS 50000

// Action function
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
double init_weight()
{
  return ( (double) rand() ) / ( (double) RAND_MAX);
}

void shuffle(int *array, size_t n)
{
  if (n > 1)
    {
      size_t i;
      for (i = 0; i < n - 1; i++)
	{
	  size_t j = i + rand() / ( RAND_MAX / (n - i) + 1);
	  int t = array[j];
	  array[j] = array[i];
	  array[i] = t;
	}
    }
}

int main()
{
  static const int numInputs = 2;
  static const int numHiddenNodes = 2;
  static const int numOutputs = 1;

  double hiddenLayer[numHiddenNodes];
  double outputLayer[numOutputs];

  double hiddenLayerBias[numHiddenNodes];
  double outputLayerBias[numOutputs];

  double hiddenWeights[numInputs][numHiddenNodes];
  double outputWeights[numHiddenNodes][numOutputs];

  static const int numTrainingSets = 4;

  double training_inputs[4][2] =
    { {0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}, {1.0, 1.0} };

  double training_outputs[4][1] =
    { {0.0f}, {1.0f}, {1.0f}, {0.0f} };


  for (int i = 0; i < numInputs; i++)
    {
      for (int j = 0; j < numHiddenNodes; j++)
	{
	  hiddenWeights[i][j] = init_weight();
	}
    }

  for (int i = 0; i < numHiddenNodes; i++)
    {
      hiddenLayerBias[i] = init_weight();
      for (int j = 0; j < numOutputs; j++)
	{
	  outputWeights[i][j] = init_weight();
	}
    }

  for (int i = 0; i < numOutputs; i++)
    {
      outputLayerBias[i] = init_weight();
    }
  
  int trainingSetOrder[] = {0,1,2,3};

  for (int n = 0; n < EPOCHS; n++)
    {
      shuffle(trainingSetOrder, numTrainingSets);

      for (int x = 0; x < numTrainingSets; x++)
	{
	  int i = trainingSetOrder[x];
	
	  for (int j = 0; j < numHiddenNodes; j++)
	    {
	      double activation = hiddenLayerBias[j];
	      for (int k = 0; k < numInputs ; k++)
		{
		  activation += training_inputs[i][k] * hiddenWeights[k][j];
		}

	      hiddenLayer[j] = sigmoid(activation);
	    }

	  for (int j = 0; j < numOutputs; j++)
	    {
	      double activation = outputLayerBias[j];
	      for (int k = 0; k < numHiddenNodes; k++)
		{
		  activation += hiddenLayer[k] * outputWeights[k][j];
		}
	      outputLayer[j] = sigmoid(activation);
	    }


	  printf("Input: %f %f    Output: %f   Expected Output: %f \n",
		 training_inputs[i][0], training_inputs[i][1],
		 outputLayer[0], training_outputs[i][0]);

	  // Backprop
	  
	  // Compute change in output weights
	  double DeltaOutput[numOutputs];
	  for (int j = 0; j < numOutputs; j++)
	    {
	      double dError = (training_outputs[i][j] - outputLayer[j]);
	      DeltaOutput[j] = dError * dSigmoid(outputLayer[j]);
	    }

	  // Compute change in hidden weights
	  double deltaHidden[numHiddenNodes];
	  for (int j = 0; j < numHiddenNodes; j++)
	    {
	      double dError = 0.0f;
	      for (int k = 0; k < numOutputs; k++)
		{
		  dError += DeltaOutput[k] * outputWeights[j][k];
		}
	      deltaHidden[j] = dError * dSigmoid(hiddenLayer[j]);
	    }

	  // Apply change in output weights
	  for (int j = 0; j < numOutputs; j++)
	    {
	      outputLayerBias[j] += DeltaOutput[j]*LR;
	      for (int k = 0; k < numHiddenNodes; k++)
		{
		  outputWeights[k][j] += hiddenLayer[k] * DeltaOutput[j] * LR;
		}
	    }

	  // Apply change in hidden weights
	  for (int j = 0; j < numHiddenNodes; j++)
	    {
	      hiddenLayerBias[j] += deltaHidden[j]*LR;
	      for (int k = 0; k < numInputs; k++)
		{
		  hiddenWeights[k][j] += training_inputs[i][k] * deltaHidden[j] * LR;
		}
	    }
	}
    }

  // Print weights
  printf("Final Hidden Weights\n[ ");
  for (int j=0; j<numHiddenNodes; j++) {
    printf("[ ");
    for(int k=0; k<numInputs; k++) {
      printf("%f ", hiddenWeights[k][j]);
    }
    printf("] ");
  }
  printf("]\n");
    
  printf("Final Hidden Biases\n[ ");
  for (int j=0; j<numHiddenNodes; j++) {
    printf("%f ", hiddenLayerBias[j]);
	
  }
  printf("]\n");
  printf("Final Output Weights");
  for (int j=0; j<numOutputs; j++) {
    printf("[ ");
    for (int k=0; k<numHiddenNodes; k++) {
      printf("%f", outputWeights[k][j]);
    }
    printf("]\n");
  }
  printf("Final Output Biases\n[ ");
  for (int j=0; j<numOutputs; j++) {
    printf("%f ", outputLayerBias[j]);
        
  }
  printf("]\n");


  return 0;
}