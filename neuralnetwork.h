//*********************************************************************************
//
// neuralnetwork.h
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
//
//*********************************************************************************

#ifndef NN_H_
#define NN_H_

using namespace std;

class NeuralNetworkLayer;

class NeuralNetwork
{
private:

    uint32_t totalNumberOfLayers;
    double *inputLayer;
    NeuralNetworkLayer **hiddenLayers;
    NeuralNetworkLayer *outputLayer;
    double gain;
    double momentum;

public:

    NeuralNetwork(uint32_t p_totalnoOfLayers, uint32_t *p_neuronsInEachLayer, double p_gain = 1, double p_momentum = 1);
    void initializeWeights(uint32_t p_totalNumberOfLayers, uint32_t *p_neuronsInEachLayer, NeuralNetworkLayer **p_layer, NeuralNetworkLayer *p_output_layer);
    void calculateInputSum(double *p_input,uint32_t previousNeuronCount, double** p_weightMatrix, uint32_t currentNeuronCount, double* currentLayerInputSum);
    void forwardPropogate(uint32_t p_totalNumberOfLayers, uint32_t *p_neuronsInEachLayer, NeuralNetworkLayer **p_layer);


    void calculateActivation();
    void calculateOutputError();
    void calculateDelta();
    void forwardPropogate();
    void backPropogate();
    void updateWeights();


};

class NeuralNetworkLayer
{
private:

    uint32_t numberOfNeurons;
    double **currentLayerWeightMatrix;
    double **currentLayerUpdateMatrix;
    double *currentLayerError;
    double *currentLayerActivation;
    double *currentLayerInputSum;

public:

    NeuralNetworkLayer(uint32_t *p_neuronsInEachLayer, uint32_t p_currentHiddenLayer);
    friend void NeuralNetwork::initializeWeights(uint32_t , uint32_t*, NeuralNetworkLayer**, NeuralNetworkLayer*);
    friend void NeuralNetwork::forwardPropogate(uint32_t p_totalNumberOfLayers, uint32_t *p_neuronsInEachLayer, NeuralNetworkLayer **p_layer);

};


#endif /* NN_H_ */
