#pragma once
#include <stdint.h>
#include <vector>
#include <assert.h>
#include <stdlib.h>
#include <ctime>
#include <random>

class NeuralNetwork {
public:

private:
	int32_t numberOfInputs;
	int32_t numberOfHidden;
	int32_t numberOfOutputs;

	std::vector<double> inputNodes;
	std::vector<double> hiddenNodes;
	std::vector<double> outputNodes;

	std::vector<double> inputHiddenWeights;
	std::vector<double> outputHiddenWeights;

	std::vector<int32_t> clampedOutputs;

};