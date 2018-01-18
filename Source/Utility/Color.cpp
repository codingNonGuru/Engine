#include <iostream>

#include "Utility/Color.hpp"

Color::Color() {}

Color::Color(float value) : r_(value), g_(value), b_(value), a_(value)
{
}

Color::Color(float r, float g, float b) : r_(r), g_(g), b_(b), a_(1.0f)
{
}

Color::Color(float r, float g, float b, float a) : r_(r), g_(g), b_(b), a_(a)
{
}

Color Color::operator + (Color& other)
{
	Color result;

	result.r_ = this->r_ + other.r_;
	result.g_ = this->g_ + other.g_;
	result.b_ = this->b_ + other.b_;
	result.a_ = this->a_ + other.a_;

	return result;
}

Color Color::operator * (float factor) const
{
	Color result;

	result.r_ = r_ * factor;
	result.g_ = g_ * factor;
	result.b_ = b_ * factor;
	result.a_ = a_ * factor;

	return result;
}

Color Color::operator - (Color& other)
{
	Color result;

	result.r_ = r_ - other.r_;
	result.g_ = g_ - other.g_;
	result.b_ = b_ - other.b_;
	result.a_ = a_ - other.a_;

	return result;
}