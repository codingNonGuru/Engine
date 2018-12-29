#pragma once

#include "Conventions.hpp"

class DataBuffer;
class Color;

class Stencil
{
	DataBuffer* buffer_;

	Size size_;

public:
	Stencil();

	Stencil(Size, DataBuffer*, Size, Size);

	void Bind(Index);

	Size GetSize() const;

	void Apply(DataBuffer*, Size, Float, Color&, Size);
};
