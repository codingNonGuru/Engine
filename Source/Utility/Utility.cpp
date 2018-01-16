#include "Utility/Utility.hpp"

std::random_device randomDevice;
std::mt19937 twister(randomDevice());

namespace utility
{
	float GetRandom(float minimum, float maximum)
	{
		std::uniform_real_distribution<float> distribution(minimum, maximum);
		return distribution(twister);
	}

	int GetRandom(int minimum, int maximum)
	{
		std::uniform_int_distribution<int> distribution(minimum, maximum);
		return distribution(twister);
	}

	float GetRandom(float minValue, float maxValue, float expectedValue, float standardDeviation)
	{
		float value = GetRandom(minValue, maxValue);
		float gaussChance = 1.0f - exp(-pow(value - expectedValue, 2.0f) / (2.0f * pow(standardDeviation, 2.0f)));

		if(RollDice<float>(gaussChance) == true)
			return GetRandom(minValue, maxValue, expectedValue, standardDeviation);
		else
			return value;
	}
}
