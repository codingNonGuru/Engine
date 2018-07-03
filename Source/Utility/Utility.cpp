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

	float GetSaturation(Color color)
	{
		Color result;

		float average = (color.r_ + color.g_ + color.b_) / 3.0f;
		float maxRate;
		if(color.r_ > average)
			result.r_ = abs(average - color.r_) / (1.0f - average);
		else
			result.r_ = abs(average - color.r_) / average;

		if(color.g_ > average)
			result.g_ = abs(average - color.g_) / (1.0f - average);
		else
			result.g_ = abs(average - color.g_) / average;

		if(color.b_ > average)
			result.b_ = abs(average - color.b_) / (1.0f - average);
		else
			result.b_ = abs(average - color.b_) / average;

		if(result.r_ > result.g_)
			maxRate = result.r_;
		else
			maxRate = result.g_;
		if(result.b_ > maxRate)
			maxRate = result.b_;

		return maxRate;
	}

	Color SetSaturation(Color color, float rate)
	{
		Color result;

		float average = (color.r_ + color.g_ + color.b_) / 3.0f;
		float maxRate;
		if(color.r_ > average)
			result.r_ = abs(average - color.r_) / (1.0f - average);
		else
			result.r_ = abs(average - color.r_) / average;

		if(color.g_ > average)
			result.g_ = abs(average - color.g_) / (1.0f - average);
		else
			result.g_ = abs(average - color.g_) / average;

		if(color.b_ > average)
			result.b_ = abs(average - color.b_) / (1.0f - average);
		else
			result.b_ = abs(average - color.b_) / average;

		if(result.r_ > result.g_)
			maxRate = result.r_;
		else
			maxRate = result.g_;
		if(result.b_ > maxRate)
			maxRate = result.b_;

		result.r_ = abs(((average - color.r_) / maxRate) * rate - average);
		if(result.r_ > 1.0f)
			result.r_ = 1.0f;
		result.g_ = abs(((average - color.g_) / maxRate) * rate - average);
		if(result.g_ > 1.0f)
			result.g_ = 1.0f;
		result.b_ = abs(((average - color.b_) / maxRate) * rate - average);
		if(result.b_ > 1.0f)
			result.b_ = 1.0f;

		return result;
	}

	Color AddSaturation(Color color, float amount)
	{
		Color result;

		float average = (color.r_ + color.g_ + color.b_) / 3.0f;
		float maxRate;
		if(color.r_ > average)
			result.r_ = abs(average - color.r_) / (1.0f - average);
		else
			result.r_ = abs(average - color.r_) / average;

		if(color.g_ > average)
			result.g_ = abs(average - color.g_) / (1.0f - average);
		else
			result.g_ = abs(average - color.g_) / average;

		if(color.b_ > average)
			result.b_ = abs(average - color.b_) / (1.0f - average);
		else
			result.b_ = abs(average - color.b_) / average;

		if(result.r_ > result.g_)
			maxRate = result.r_;
		else
			maxRate = result.g_;
		if(result.b_ > maxRate)
			maxRate = result.b_;

		float rate = amount + maxRate;
		if(rate > 1.0f)
			rate = 0.9999f;
		if(rate < 0.0f)
			rate = 0.0f;

		result.r_ = abs(((average - color.r_) / maxRate) * rate - average);
		if(result.r_ > 1.0f)
			result.r_ = 1.0f;
		result.g_ = abs(((average - color.g_) / maxRate) * rate - average);
		if(result.g_ > 1.0f)
			result.g_ = 1.0f;
		result.b_ = abs(((average - color.b_) / maxRate) * rate - average);
		if(result.b_ > 1.0f)
			result.b_ = 1.0f;

		return result;
	}

	Color Lighten(Color color, float rate)
	{
		Color result;

		if(rate < 0.0f)
		{
			result = color * (1.0f + rate);
		}
		else
		{
			result = (Color(1.0f) - color) * rate + color;

			//(Color(1.0f) - color) * rate + color;
		}

		return result;
	}

	float Interpolate(float t)
	{
		return 3.0f * t * t - 2.0f * t * t * t;
	}
}
