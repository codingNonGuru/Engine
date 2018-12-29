#pragma once

#include "Conventions.hpp"

class Stencil;

class StencilManager
{
	struct Identifier
	{
		Word Name_;

		Index Index_;

		Identifier() {}

		Identifier(Word Name, Index Index) : Name_(Name), Index_(Index) {}

		bool operator==(Identifier& other) {return Name_ == other.Name_ && Index_ == other.Index_;}
	};

	static Map <Stencil*, Identifier> stencils_;

public:
	static void Add(Stencil*, Word, Index);

	static Stencil* Get(Word, Index);
};
