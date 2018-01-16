#pragma once

#include "Conventions.hpp"
#include "Types.hpp"

class Kernel
{
private:
	Grid <float> values_;

	Array <float> weights_;

	int side_;

	float sum_;

public:
	Kernel();

	Kernel(int side);

	int GetSide();

	int GetSpread();

	Grid <float> & GetValues();

	float* get(int x, int y);

	float* operator() (int x, int y);

	void Initialize(KernelTypes type, float variable, float contrastStrength, bool normalized);

	void AddLayer(KernelTypes type, float variable, float contrastStrength, bool normalized, float weight);

	Array <float> & GetWeights();
};
