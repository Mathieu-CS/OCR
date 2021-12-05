#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

double sigmoid(double x);
double dSigmoid(double x);
double init_weight();
//void shuffle(int *array, size_t n);
//void save(char filename[], double hiddenWeights[numInputs][numHiddenNodes], double hiddenLayerBias[numHiddenNodes], double outputWeights[numHiddenNodes][numOutputs], double outputLayerBias[numOutputs]);
void loadTraining(char path[], double arr[]);
void neuralNetwork();




#endif
