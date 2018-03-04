#pragma once

#include "Conventions.hpp"

class Color
{
public:
	float r_, g_, b_, a_;

	Color();

	Color(float);

	Color(float, float, float);

	Color(float, float, float, float);

	Color operator + (Color&);

	Color operator * (float) const;

	Color operator - (Color&);

	operator Float4 ();

	static Color BLACK;

	static Color WHITE;

	static Color RED;

	static Color DARK_RED;
};
