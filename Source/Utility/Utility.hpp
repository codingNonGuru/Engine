#pragma once

#include <math.h>
#include <random>

#include "Conventions.hpp"
#include "Utility/Color.hpp"

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

	float GetSaturation(Color);

	Color SetSaturation(Color, float);

	Color AddSaturation(Color, float);

	Color Lighten(Color, float);

	float Interpolate(float);

	void Clamp(float &, float = 0.0f, float = 1.0f);
}
