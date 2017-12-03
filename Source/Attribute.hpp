#pragma once

#include "Types.hpp"

struct AttributeType
{
	LongWord Identifier_;

	Length MemorySize_;

	Length ComponentCount_;

	AttributeElementTypes ElementType_;

	AttributeType() {}

	AttributeType(LongWord identifier, Length memorySize, Length componentCount, AttributeElementTypes type) : Identifier_(identifier), MemorySize_(memorySize), ComponentCount_(componentCount), ElementType_(type) {}
};

class Attribute
{

};
