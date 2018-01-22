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

	static Color Black_;

	static Color White_;

	static Color Red_;

	static Color DarkRed_;
};
