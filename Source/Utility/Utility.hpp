#pragma once

#include <math.h>
#include <random>

#include "Conventions.hpp"

namespace utility
{
	float GetRandom(float minimum, float maximum);

	int GetRandom(int min, int max);

	float GetRandom(float, float, float, float);

	template<typename ValueType>
	bool RollDice(ValueType chance)
	{
		return GetRandom(0.0f, 1.0f) < chance;
	}
}
