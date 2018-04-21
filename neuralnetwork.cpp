//*********************************************************************************
//
// neuralnetwork.cpp
//
// This is the header file for the Neural network library.
// It contains the class NeuralNetwork and the interface for the class
//
//
//      Date                Author                          Comments
//
// |  Apr 19, 2017  |    Naveen B Kori          |                                 |
// |                |                           |                                 |
// |                |                           |                                 |
// |                |                           |                                 |
//*********************************************************************************
#include<stdint.h>
#include<nn.h>
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<Uart0.h>
#include <ctime>



using namespace std;


extern void putsUart0(char* str);
extern void putcUart0(char c);
extern char getcUart0();

NeuralNetworkLayer::NeuralNetworkLayer(uint32_t *p_neuronsInEachLayer, uint32_t p_currentHiddenLayer)
{
    numberOfNeurons = p_neuronsInEachLayer[p_currentHiddenLayer];
    currentLayerError = (double*) malloc(p_neuronsInEachLayer[p_currentHiddenLayer] * sizeof(double));
    currentLayerActivation = (double*) malloc(p_neuronsInEachLayer[p_currentHiddenLayer] * sizeof(double));
    currentLayerInputSum = (double*) malloc(p_neuronsInEachLayer[p_currentHiddenLayer] * sizeof(double));
    currentLayerWeightMatrix = (double**) malloc(p_neuronsInEachLayer[p_currentHiddenLayer-1] * sizeof(double*));

    uint32_t currentNeuron = 0;

    for(currentNeuron = 0; currentNeuron < (p_neuronsInEachLayer[p_currentHiddenLayer-1]); currentNeuron++)
    {
        currentLayerWeightMatrix[currentNeuron] = (double*) malloc(p_neuronsInEachLayer[p_currentHiddenLayer] * sizeof(double));
    }

    currentLayerUpdateMatrix = (double**) malloc(p_neuronsInEachLayer[p_currentHiddenLayer-1] * sizeof(double*));

    for(currentNeuron = 0; currentNeuron < (p_neuronsInEachLayer[p_currentHiddenLayer-1]); currentNeuron++)
    {
        currentLayerUpdateMatrix[currentNeuron] = (double*) malloc(p_neuronsInEachLayer[p_currentHiddenLayer] * sizeof(double));
    }


}


void NeuralNetwork::initializeWeights(uint32_t p_totalNumberOfLayers, uint32_t *p_neuronsInEachLayer, NeuralNetworkLayer **p_layer, NeuralNetworkLayer *p_output_layer)
{
    srand(time(NULL));

    uint32_t currentLayer, previousLayer, currentLayerNeuron, previousLayerNeuron;

    for(currentLayer = 1; currentLayer < p_totalNumberOfLayers; currentLayer++ )
    {
        previousLayer = currentLayer-1;

        for(previousLayerNeuron = 0; previousLayerNeuron < p_neuronsInEachLayer[previousLayer]; previousLayerNeuron++)
        {
            for(currentLayerNeuron = 0; currentLayerNeuron < p_neuronsInEachLayer[currentLayer]; currentLayerNeuron++)
            {
                (p_layer[currentLayer])->currentLayerWeightMatrix[previousLayerNeuron][currentLayerNeuron] = rand();

            }
        }
    }
}


NeuralNetwork::NeuralNetwork(uint32_t p_totalnumberOfLayers, uint32_t *p_neuronsInEachLayer, double p_gain, double p_momentum)
{

    totalNumberOfLayers = p_totalnumberOfLayers;
    inputLayer = (double*) malloc(p_neuronsInEachLayer[0]*sizeof(double));
    hiddenLayers = (NeuralNetworkLayer**) malloc((p_totalnumberOfLayers-2)*(sizeof(NeuralNetworkLayer*)));
    outputLayer = new NeuralNetworkLayer(p_neuronsInEachLayer, (totalNumberOfLayers-1));
    gain = p_gain;
    momentum = p_momentum;

    uint32_t currentHiddenLayer = 0;

    for(currentHiddenLayer = 0; currentHiddenLayer <= (totalNumberOfLayers-2); currentHiddenLayer++ )
    {
        hiddenLayers[currentHiddenLayer] = new NeuralNetworkLayer(p_neuronsInEachLayer, (currentHiddenLayer+1));

    }

    initializeWeights(totalNumberOfLayers, p_neuronsInEachLayer, hiddenLayers, outputLayer);

}


void NeuralNetwork::forwardPropogate(uint32_t p_totalNumberOfLayers, uint32_t *p_neuronsInEachLayer, NeuralNetworkLayer **p_layer)
{
    uint32_t currentLayer, previousLayer;
    double *previousLayerActivation, *currentLayerSum;
    double **currentWeightMatrix;

    for(currentLayer = 1; currentLayer < p_totalNumberOfLayers; currentLayer++)
    {
        previousLayer = currentLayer - 1;

        if(currentLayer == 1)
            previousLayerActivation = inputLayer;
        else
            previousLayerActivation = hiddenLayers[previousLayer-1]->currentLayerActivation;

        if(currentLayer == p_totalNumberOfLayers-1)
        {
            currentWeightMatrix = outputLayer->currentLayerWeightMatrix;
            currentLayerSum = outputLayer->currentLayerInputSum;
        }

        else
        {
            currentWeightMatrix = hiddenLayers[currentLayer-1]->currentLayerWeightMatrix;
            currentLayerSum = hiddenLayers[currentLayer-1]->currentLayerInputSum;
        }

        calculateInputSum(previousLayerActivation, p_neuronsInEachLayer[previousLayer], currentWeightMatrix, p_neuronsInEachLayer[currentLayer], currentLayerSum);


    }
}


void NeuralNetwork::calculateInputSum(double *p_input,uint32_t previousNeuronCount, double** p_weightMatrix, uint32_t currentNeuronCount, double* currentLayerInputSum)
{
    uint32_t currentCount, previousCount;
    double sum;

    for(currentCount = 0; currentCount < currentNeuronCount; currentCount++)
    {
        sum = 0;
        for(previousCount = 0; previousCount < previousNeuronCount; previousCount++)
        {
            sum += p_input[previousCount] * p_weightMatrix[previousCount][currentCount];
        }

        currentLayerInputSum[currentCount] = sum;
    }

}

void NeuralNetwork::calculateActivation(uint32_t p_totalNumberOfLayers, uint32_t *p_neuronsInEachLayer)
{
    uint32_t currentCount, previousCount;
        double sum;

        for(currentCount = 0; currentCount < currentNeuronCount; currentCount++)
        {
            sum = 0;
            for(previousCount = 0; previousCount < previousNeuronCount; previousCount++)
            {
                sum += p_input[previousCount] * p_weightMatrix[previousCount][currentCount];
            }

            currentLayerInputSum[currentCount] = sum;
        }

    
    
}




