#pragma once

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
};
